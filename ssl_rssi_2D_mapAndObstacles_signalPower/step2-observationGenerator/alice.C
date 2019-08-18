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

double Power_node_ref = 30.+10.*log10(0.1); // dBm

double mins[3] = {-100,-100,0};
double maxs[3] = {100,100,0};

void GetMap(ToolSignalPowerGenerator * spg);
void generateObservations(ToolSignalPowerGenerator * spg);
bool IsAnchorPositionGood(vector<double> A);
bool IsNodePositionGood(vector<double> A);

int main()
{
	cout<<"Hello "<<endl;


	// step 1 : generate samples
	ToolMapGenerator * map = new ToolMapGenerator("Map", "3D");
	map->SetSigma(10);
	map->ImportSamples();

	// step 2 
	ToolSignalPowerGenerator * spg = new ToolSignalPowerGenerator("spg");
	spg->SetToolMapGenerator(map);
	spg->SetFactor(1e-1);


	//
	// output observaions based on Shadowing model
	//

	generateObservations(spg);

	return 1;
}

void generateObservations(ToolSignalPowerGenerator * spg)
{
	//  generate observations
	default_random_engine engine_(time(0));
	uniform_real_distribution<double> randomx(mins[0], maxs[0]);
	uniform_real_distribution<double> randomy(mins[1], maxs[1]);

	int Number_nodes = 10;
	int Number_anchors = 40;

	vector<double> A_anchor_x, A_node_x;
	vector<double> A_anchor_y, A_node_y;
	vector<double> A_anchor_z, A_node_z;

	for(int i=0;i<Number_anchors;i++)
	{
		int anID = i;

		vector<double> A_An;
		bool isAnchorGet = false;

		while(!isAnchorGet)
		{
			A_An.clear();

			double x = randomx(engine_);
			double y = randomy(engine_);

			A_An.push_back(x);
			A_An.push_back(y);
			A_An.push_back(0);

			if(IsAnchorPositionGood(A_An)) isAnchorGet=true;
		}

		cout<<"Anchor : "<<A_An[0]<<", "<<A_An[1]<<", "<<A_An[2]<<endl;
		A_anchor_x.push_back(A_An[0]);
		A_anchor_y.push_back(A_An[1]);
		A_anchor_z.push_back(A_An[2]);

	}

	for(int j=0;j<Number_nodes;j++)
	{
		int noID = j;

		vector<double> A_No;
		bool isNodeGet = false;

		while(!isNodeGet)
		{
			A_No.clear();

			double x = randomx(engine_);
			double y = randomy(engine_);

			A_No.push_back(x);
			A_No.push_back(y);
			A_No.push_back(0);

			if(IsNodePositionGood(A_No)) isNodeGet=true;
		}

		cout<<"Node : "<<A_No[0]<<", "<<A_No[1]<<", "<<A_No[2]<<endl;
		A_node_x.push_back(A_No[0]);
		A_node_y.push_back(A_No[1]);
		A_node_z.push_back(A_No[2]);
	}

	ofstream write2("observations.txt");
	int NumberAnchors = Number_anchors;
	int NumberNodes = Number_nodes;
	write2<<"NumberAnchors "<<NumberAnchors<<" NumberNodes "<<NumberNodes<<endl; 

	for(int i=0;i<Number_anchors;i++)
	for(int j=0;j<Number_nodes;j++)
	{
		int anID = i;
		int noID = j;

		vector<double> A_anchor, A_node;
		A_anchor.push_back(A_anchor_x[anID]);
		A_anchor.push_back(A_anchor_y[anID]);
		A_anchor.push_back(A_anchor_z[anID]);

		A_node.push_back(A_node_x[noID]);
		A_node.push_back(A_node_y[noID]);
		A_node.push_back(A_node_z[noID]);

		double d_x = A_anchor_x[anID]-A_node_x[noID];
		double d_y = A_anchor_y[anID]-A_node_y[noID];
		double d_z = A_anchor_z[anID]-A_node_z[noID];

		double distance = sqrt(d_x*d_x + d_y*d_y + d_z*d_z);

		double Power_node = Power_node_ref;
		cout<<"Power_node_ref "<<Power_node_ref<<endl;
		double Strength = spg->GetSignalPowerStrength_dBm(Power_node,A_anchor,A_node);

		write2<<anID<<" "<<A_anchor_x[anID]<<" "<<A_anchor_y[anID]<<" "<<A_anchor_z[anID]<<" "<<Strength<<" "<<noID<<" "<<A_node_x[noID]<<" "<<A_node_y[noID]<<" "<<A_node_z[noID]<<" "<<Power_node<<endl;
	}
	write2.close();
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

bool IsAnchorPositionGood(vector<double> A)
{
	double x = A[0];
	double y = A[1];

	double thickness = 20;

	if(x<maxs[0]-thickness&&x>mins[0]+thickness)
	if(y<maxs[1]-thickness&&y>mins[1]+thickness)
	{
		return false;
	}

	return true;
}

bool IsNodePositionGood(vector<double> A)
{

	return true;
}
