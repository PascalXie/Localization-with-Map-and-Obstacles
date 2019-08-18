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

#include "ToolMapGenerator.hh"
#include "ToolSignalPowerGenerator.hh"


using namespace std;

vector<int> AnchorIDs;
vector<int> NodeIDs;
vector<double> S_anchors, S_nodes;
vector<double> axs, ays, xxs, xys;

int NumberAnchors = 0;
int NumberNodes = 0;

double Power_node;
double Power_ref;
double PathLossExponent;
double d0;

bool PowerRSSI_residualTest();
bool PowerRSSI();
bool PowerRSSI_singleNode(int NodeID,vector<double> &results);
int RSSI();

int main()
{
	cout<<"Hello "<<endl;

	//
	string filename1 = "../Data_PathLossExponent.txt";
	ifstream file1(filename1.c_str());

	if(file1.fail())
	{
		cout<<"Can not find the file1 \" "<<filename1<<" \""<<endl;
		return 0;
	}

	string temp1;

	file1>>temp1>>Power_node;
	file1>>temp1>>Power_ref;
	file1>>temp1>>PathLossExponent;
	file1>>temp1>>d0;

	cout<<"Power_node "<<Power_node<<endl;
	cout<<"Power_ref "<<Power_ref<<endl;
	cout<<"PathLossExponent "<<PathLossExponent<<endl;
	cout<<"d0 "<<d0<<endl;


	//
	string filename = "../observations.txt";
	ifstream file(filename.c_str());

	if(file.fail())
	{
		cout<<"Can not find the file \" "<<filename<<" \""<<endl;
		return 0;
	}

	string temp;
	file>>temp>>NumberAnchors>>temp>>NumberNodes;
	cout<<"NumberAnchors "<<NumberAnchors<<" ; NumberNodes "<<NumberNodes<<endl;

	int AnchorID = 0;
	int NodeID = 0;
	double ax = 0; 
	double ay = 0; 
	double az = 0;
	double xx = 0;
	double xy = 0;
	double xz = 0;
	double S_anchorOb = 0; // signal Power
	double S_node = 0; // signal power

	while(!file.eof())
	{
		file>>AnchorID>>ax>>ay>>az>>S_anchorOb>>NodeID>>xx>>xy>>xz>>S_node;

		if(file.eof()) break;

		//cout<<"AnchorID "<<AnchorID<<", ax "<<ax<<", ay "<<ay<<", az "<<az<<"; Power "<<S_anchorOb<<endl;
		//cout<<"NodeID "<<NodeID<<", xx "<<xx<<", xy "<<xy<<", xz "<<xz<<"; Power "<<S_node<<endl;

		AnchorIDs.push_back(AnchorID);
		NodeIDs.push_back(NodeID);
		axs.push_back(ax);
		ays.push_back(ay);
		xxs.push_back(xx);
		xys.push_back(xy);
		S_anchors.push_back(S_anchorOb);
		S_nodes.push_back(S_node);
	}

	file.close();

	// get observations
	ofstream file3("log_OptimizationResults.txt");
	for(int j=0;j<NumberNodes;j++)
	{
		int NodeID = j;
		vector<double> results;
		PowerRSSI_singleNode(NodeID,results);

		cout<<"NodeID "<<NodeID<<", Node Position : "<<results[0]<<", "<<results[1]<<endl;
		file3<<NodeID<<" "<<results[0]<<" "<<results[1]<<endl;
	}
	file3.close();




	//
	//bool isBuild1Good = PowerRSSI_residualTest();
	//bool isBuildGood = PowerRSSI();

	return 1;
}

bool PowerRSSI_residualTest()
{
	int NumberNodes = 2;
	int observationsSize = 4+2*NumberNodes;
	int varialbeSize = 2*NumberNodes;
	int residualSize = 1;

	vector<double> obs;
	obs.push_back(-38.4748);
	obs.push_back(Power_ref);
	obs.push_back(-44.8991);
	obs.push_back(Power_ref);
	obs.push_back(PathLossExponent);
	obs.push_back(d0);
	obs.push_back(-45.3566);
	obs.push_back(84.0258);

	vector<double> variables;
	variables.push_back(-93.8696);
	variables.push_back(27.0384);
	variables.push_back(46.2762);
	variables.push_back(-43.2151);

	vector<double> residual;

	PolyResidualBlockFunction *poly = new PolyResidualBlockFunction("poly",obs,observationsSize,varialbeSize,residualSize);
	bool isPolyGood = poly->ResidualFunction(variables,residual);

	cout<<"Residual "<<residual[0]<<endl;

	return true;
}

//
bool PowerRSSI_singleNode(int NodeID,vector<double> &results)
{
	//
	// Optimization 
	//
	int observationsSize = 4 + 2*1;
	int residualSize = 1;
	int varialbeSize = 2*1;

	string managerName = "ManagerNode_" + to_string(NodeID);
	UserOptimizationManager * manager = new NewtonsOptimizationManager(managerName,observationsSize,varialbeSize,residualSize);

	// set variables
	vector<double> variables;
	for(int i=0;i<varialbeSize;i++)
	{
		variables.push_back(0.);
	}
	manager->SetUserInitialization(variables);

	// set cost function
	UserCostFunction* costFunction = new NewtonsCostFunction("costFunction",observationsSize,varialbeSize,residualSize);

	// get observations
	for(int i=0;i<NumberAnchors;i++)
	{
		int AnchorID = i;
		vector<double> observation_current;

		int S_anchorObID = AnchorID*NumberNodes + NodeID;
		observation_current.push_back(S_anchors[S_anchorObID]);
		observation_current.push_back(Power_ref);

		int ID = AnchorID*NumberNodes + 0 ;
		observation_current.push_back(PathLossExponent);
		observation_current.push_back(d0);
		observation_current.push_back(axs[ID]);
		observation_current.push_back(ays[ID]);
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
	double UserReferencedEpsilon = 1e-7;
	manager->SetUserEpsilonForTerminating(UserReferencedEpsilon);

	// initialize
	cout<<" "<<endl;
	cout<<"Initialize "<<endl;
	manager->Initialize();

	// get results
	manager->GetVariables(results);

	return true;
}

bool PowerRSSI()
{
	string filename = "../observations.txt";
	ifstream file(filename.c_str());

	if(file.fail())
	{
		cout<<"Can not find the file \" "<<filename<<" \""<<endl;
		return 0;
	}

	int NumberAnchors = 0;
	int NumberNodes = 0;
	string temp;
	file>>temp>>NumberAnchors>>temp>>NumberNodes;
	cout<<"NumberAnchors "<<NumberAnchors<<" ; NumberNodes "<<NumberNodes<<endl;

	int AnchorID = 0;
	int NodeID = 0;
	double ax = 0; 
	double ay = 0; 
	double az = 0;
	double xx = 0;
	double xy = 0;
	double xz = 0;
	double S_anchorOb = 0; // signal Power
	double S_node = 0; // signal power

	vector<int> AnchorIDs;
	vector<int> NodeIDs;
	vector<double> S_anchors, S_nodes;
	vector<double> axs, ays, xxs, xys;

	while(!file.eof())
	{
		file>>AnchorID>>ax>>ay>>az>>S_anchorOb>>NodeID>>xx>>xy>>xz>>S_node;

		if(file.eof()) break;

		cout<<"AnchorID "<<AnchorID<<", ax "<<ax<<", ay "<<ay<<", az "<<az<<"; Power "<<S_anchorOb<<endl;
		cout<<"NodeID "<<NodeID<<", xx "<<xx<<", xy "<<xy<<", xz "<<xz<<"; Power "<<S_node<<endl;

		AnchorIDs.push_back(AnchorID);
		NodeIDs.push_back(NodeID);
		axs.push_back(ax);
		ays.push_back(ay);
		xxs.push_back(xx);
		xys.push_back(xy);
		S_anchors.push_back(S_anchorOb);
		S_nodes.push_back(S_node);
	}

	file.close();


	//
	// Optimization 
	//
	int observationsSize = 4 + 2*NumberNodes;
	int residualSize = 1;
	int varialbeSize = 2*NumberNodes;

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

	// get observations
	for(int i=0;i<NumberAnchors;i++)
	{
		int AnchorID = i;
		vector<double> observation_current;
		for(int j=0;j<NumberNodes;j++)
		{
			int NodeID = j;
			int S_anchorObID = AnchorID*NumberNodes + NodeID;
			observation_current.push_back(S_anchors[S_anchorObID]);
			observation_current.push_back(Power_ref);
		}
		int ID = AnchorID*NumberNodes + 0 ;
		observation_current.push_back(PathLossExponent);
		observation_current.push_back(d0);
		observation_current.push_back(axs[ID]);
		observation_current.push_back(ays[ID]);
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
	double UserReferencedEpsilon = 1e-7;
	manager->SetUserEpsilonForTerminating(UserReferencedEpsilon);

	// initialize
	cout<<" "<<endl;
	cout<<"Initialize "<<endl;
	manager->Initialize();


	return true;
}

int RSSI()
{
	string filename = "../observations.txt";
	ifstream file(filename.c_str());

	if(file.fail())
	{
		cout<<"Can not find the file \" "<<filename<<" \""<<endl;
		return 0;
	}

	int NumberAnchors = 0;
	int NumberNodes = 0;
	string temp;
	file>>temp>>NumberAnchors>>temp>>NumberNodes;
	cout<<"NumberAnchors "<<NumberAnchors<<" ; NumberNodes "<<NumberNodes<<endl;

	int AnchorID = 0;
	int NodeID = 0;
	double distanceSquared = 0;
	double ax = 0; 
	double ay = 0; 
	double xx = 0;
	double xy = 0;

	vector<int> AnchorIDs;
	vector<int> NodeIDs;
	vector<double> distancesSquared;
	vector<double> axs, ays, xxs, xys;

	while(!file.eof())
	{
		file>>AnchorID>>ax>>ay>>NodeID>>xx>>xy>>distanceSquared;

		if(file.eof()) break;

		cout<<"Distance squared "<<distanceSquared<<": Anchor ID "<<AnchorID<<", loc "<<ax<<", "<<ay<<"; Node ID "<<NodeID<<", loc "<<xx<<", "<<xy<<endl;

		AnchorIDs.push_back(AnchorID);
		NodeIDs.push_back(NodeID);
		distancesSquared.push_back(distanceSquared);
		axs.push_back(ax);
		ays.push_back(ay);
		xxs.push_back(xx);
		xys.push_back(xy);
	}

	file.close();

	//// test 
	//int observationsSize = 2 + NumberNodes;
	//int residualSize = 1;
	//int varialbeSize = 2*NumberNodes;

	//vector<double> variables;
	//variables.push_back(1.);
	//variables.push_back(2.);
	//variables.push_back(3.);
	//variables.push_back(4.);

	//vector<double> observations;
	//observations.push_back(1307.87);
	//observations.push_back(3725.57);
	//observations.push_back(62.4517);
	//observations.push_back(6.97365);
	//UserResidualBlockFunction * resi = new PolyResidualBlockFunction("test",observations,observationsSize,varialbeSize,residualSize);

	//vector<double> resiValue;
	//resi -> ResidualFunction(variables, resiValue);

	//
	// Optimization 
	//
	int observationsSize = 2 + NumberNodes;
	int residualSize = 1;
	int varialbeSize = 2*NumberNodes;

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

	// get observations
	for(int i=0;i<NumberAnchors;i++)
	{
		int AnchorID = i;
		vector<double> observation_current;
		for(int j=0;j<NumberNodes;j++)
		{
			int NodeID = j;
			int disID = AnchorID*NumberNodes + NodeID;
			observation_current.push_back(distancesSquared[disID]);
		}
		int ID = AnchorID*NumberNodes + 0 ;
		observation_current.push_back(axs[ID]);
		observation_current.push_back(ays[ID]);
		costFunction->AddResidualBlock(observation_current);

		/*
		// debug
		for(int k=0;k<2+NumberNodes;k++)
		{
			cout<<"Observation ID "<<k<<"; ";
			cout<<observation_current[k]<<", ";
		}
		cout<<endl;

		UserResidualBlockFunction * resi = new PolyResidualBlockFunction("test",observation_current,observationsSize,varialbeSize,residualSize);
		vector<double> resiValue;
		resi -> ResidualFunction(variables, resiValue);
		*/
	}

	//
	cout<<" "<<endl;
	cout<<"alice SetUserInitialization"<<endl;
	manager->SetUserInitialization(costFunction);

	//
	double UserReferencedLength = 70.;
	manager->SetUserReferencedLength(UserReferencedLength);

	// 
	double UserReferencedEpsilon = 5e3;
	manager->SetUserEpsilonForTerminating(UserReferencedEpsilon);


	// initialize
	cout<<" "<<endl;
	cout<<"Initialize "<<endl;
	manager->Initialize();


	return 1;
}
