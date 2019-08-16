#include <iostream>
#include <vector>
#include <string>
#include <fstream>

#include "UserCostFunction.hh"
#include "SteepestCostFunction.hh"
#include "NewtonsCostFunction.hh"

#include "UserResidualBlockFunction.hh"
#include "PolyResidualBlockFunction.hh"

#include "UserOptimizationManager.hh"
#include "SteepestOptimizationManager.hh"
#include "NewtonsOptimizationManager.hh"

#include "ToolNodesGenerator.hh"
#include "ToolMapGenerator.hh"
#include "ToolSignalPowerGenerator.hh"

using namespace std;

vector<double> Nxs_c, Nys_c;
vector<double> Nxs_a_1, Nys_a_1;
vector<double> Nxs_a_2, Nys_a_2;

double Power_node_ref = 30.+10.*log(0.1); // dBm

void GetMap(ToolSignalPowerGenerator * spg);
double GetRerencedStrength(ToolSignalPowerGenerator * spg);
void generateComparingData(ToolSignalPowerGenerator * spg, double pathLossExponent);

int main()
{
	cout<<"Hello "<<endl;


	// step 1 : generate samples
	ToolMapGenerator * map = new ToolMapGenerator("Map", "3D");
	map->ImportSamples();

	// step 2 
	ToolSignalPowerGenerator * spg = new ToolSignalPowerGenerator("spg");
	spg->SetToolMapGenerator(map);

	vector<double> A_anchor1, A_node1;
	A_anchor1.push_back(-100);
	A_anchor1.push_back(-100);
	A_anchor1.push_back(0);
	A_node1.push_back(100);
	A_node1.push_back(100);
	A_node1.push_back(0);

	double Power_node = Power_node_ref; // dBm
	double Power_anchor = spg->GetSignalPowerStrength_dBm(Power_node,A_anchor1,A_node1);
	cout<<"Power_anchor "<<Power_anchor<<endl;

	// referenced
	double Power_ref = GetRerencedStrength(spg);
	cout<<"Power_ref "<<Power_ref<<endl;

	// step 3  : generate map
	//GetMap(spg);

	// step 4 : path loss exponent estimation
	//
	// Optimization 
	//
	int observationsSize = 3;
	int residualSize = 1;
	int varialbeSize = 1;

	UserOptimizationManager * manager = new NewtonsOptimizationManager("NewtonsMethod",observationsSize,varialbeSize,residualSize);

	// set variables
	vector<double> variables;
	for(int i=0;i<varialbeSize;i++)
	{
		variables.push_back(0.);
	}
	manager->SetUserInitialization(variables);

	// set cost function
	UserCostFunction* costFunction = new NewtonsCostFunction("costFunction",observationsSize,varialbeSize,residualSize);

	//  observations
	double min[3] = {0,0,0};
	double max[3] = {30,0,0};

	vector<double> A_node;
	A_node.push_back(0);
	A_node.push_back(0);
	A_node.push_back(0);

	default_random_engine engine_;
	uniform_real_distribution<double> randomx(min[0], max[0]);
	uniform_real_distribution<double> randomy(min[1], max[1]);

	for(int i=0;i<100;i++)
	{
		double x = randomx(engine_);
		double y = randomy(engine_);

		vector<double> A_anchor;
		A_anchor.push_back(x);
		A_anchor.push_back(y);
		A_anchor.push_back(0);

		double Power_node = Power_node_ref;
		double Strength = spg->GetSignalPowerStrength_dBm(Power_node,A_anchor,A_node);
		double Strength_ref = Power_ref;

		double distance = sqrt((A_node[0]-A_anchor[0])*(A_node[0]-A_anchor[0])+(A_node[1]-A_anchor[1])*(A_node[1]-A_anchor[1]));

		cout<<"x "<<x<<", y "<<y<<", distance "<<distance<<", Strength "<<Strength<<", Strength_ref "<<Strength_ref<<endl;

		vector<double> observation_current;
		observation_current.push_back(Strength);
		observation_current.push_back(Strength_ref);
		observation_current.push_back(distance);
		costFunction->AddResidualBlock(observation_current);
	}

	//
	cout<<" "<<endl;
	cout<<"alice SetUserInitialization"<<endl;
	manager->SetUserInitialization(costFunction);

	//
	double UserReferencedLength = 70.;
	manager->SetUserReferencedLength(UserReferencedLength);

	// 
	double UserReferencedEpsilon = 1e-2;
	manager->SetUserEpsilonForTerminating(UserReferencedEpsilon);

	// initialize
	cout<<" "<<endl;
	cout<<"Initialize "<<endl;
	manager->Initialize();

	//
	// output results
	//
	vector<double> Variable_results;
	manager->GetVariables(Variable_results);

	double gamma = Variable_results[0];
	cout<<"gamma "<<gamma<<endl;

	generateComparingData(spg, gamma);



	return 1;
}

void generateComparingData(ToolSignalPowerGenerator * spg, double pathLossExponent)
{
	double ple = pathLossExponent;

	vector<double> A_node;
	A_node.push_back(0);
	A_node.push_back(0);
	A_node.push_back(0);

	double min[3] = {0,0,0};
	double max[3] = {30,0,0};

	int binSize = 30;

	double binWidth[3];
	for(int i=0;i<3;i++)
	{
		binWidth[i] = (max[i]-min[i])/double(binSize);
	}

	ofstream write("log_AttenuationAndPathLossExponent.txt");
	for(int i=0;i<binSize;i++)
	{
		double x = min[0] + (double(i)+0.5)*binWidth[0];
		double y = min[1] + (double(i)+0.5)*binWidth[1];

		vector<double> A_anchor;
		A_anchor.push_back(x);
		A_anchor.push_back(y);
		A_anchor.push_back(0);

		double Power_node = Power_node_ref;
		double Strength_att = spg->GetSignalPowerStrength_dBm(Power_node,A_anchor,A_node);

		double Strength_ref = GetRerencedStrength(spg);
		//cout<<"Strength_ref "<<Strength_ref<<endl;

		double distance = sqrt((A_node[0]-A_anchor[0])*(A_node[0]-A_anchor[0])+(A_node[1]-A_anchor[1])*(A_node[1]-A_anchor[1]));

		// path loss exponent
		double d_0 = 1;
		double Strength_PLE = Strength_ref - 10.*ple*log(distance/d_0);
		cout<<"Strength_att "<<Strength_att<<"; Strength_PLE "<<Strength_PLE<<endl;
		write<<distance<<" "<<Strength_att<<" "<<Strength_PLE<<endl;
	}

	write.close();
}

void GetMap(ToolSignalPowerGenerator * spg)
{
	int bin[3] = {100,100,1};
	double min[3] = {-100,-100,0};
	double max[3] = {100,100,0};

	double binWidth[3];
	for(int i=0;i<3;i++)
	{
		binWidth[i] = (max[i]-min[i])/double(bin[i]);
	}

	vector<double> A_node;
	A_node.push_back(80);
	A_node.push_back(0);
	A_node.push_back(0);

	ofstream write("Map_SignalStrengthAndPathLoss.txt");
	for(int i=0;i<bin[0];i++)
	for(int j=0;j<bin[1];j++)
	for(int k=0;k<bin[2];k++)
	{
		int xID = i;
		int yID = j;
		int zID = k;
		double x = min[0] + double(xID+0.5)*binWidth[0];
		double y = min[1] + double(yID+0.5)*binWidth[1];
		double z = min[2] + double(zID+0.5)*binWidth[2];

		//cout<<x<<" "<<y<<" "<<z<<endl;
		vector<double> A_anchor;
		A_anchor.push_back(x);
		A_anchor.push_back(y);
		A_anchor.push_back(z);

		double Power_node = Power_node_ref;
		double Strength = spg->GetSignalPowerStrength_dBm(Power_node,A_anchor,A_node);

		cout<<x<<" "<<y<<" "<<z<<" "<<Strength<<endl;

		write<<x<<" "<<y<<" "<<z<<" "<<Strength<<endl;
	}

	write.close();
}

double GetRerencedStrength(ToolSignalPowerGenerator * spg)
{
	vector<double> A_anchor, A_node;
	A_anchor.push_back(100);
	A_anchor.push_back(50);
	A_anchor.push_back(0);
	A_node.push_back(99);
	A_node.push_back(50);
	A_node.push_back(0);

	double Power_node = Power_node_ref; // dBm
	double Power_anchor = spg->GetSignalPowerStrength_dBm(Power_node,A_anchor,A_node);

	return Power_anchor;
}
