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


using namespace std;

int RSSI();
void test3();
void test4();
double observation1(double ob1);
double observation2(double ob1);

int main()
{
	cout<<"Hello "<<endl;

	// steepest descent method
	//test3();

	// Newton's method
	//test4();

	// RSSI
	int isRSSIGood = RSSI();

	return 1;
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

void test3()
{
	int observationsSize = 2;
	int residualSize = 1;
	int varialbeSize = 4;
	UserOptimizationManager * sd = new SteepestOptimizationManager("SteepestDecentMethod",observationsSize,varialbeSize,residualSize);

	// set variables
	vector<double> variables;
	variables.push_back(1.+101);
	variables.push_back(2.+101);
	variables.push_back(3.-101);
	variables.push_back(4.-101);
	sd->SetUserInitialization(variables);

	// set cost function
	UserCostFunction* costFunction = new SteepestCostFunction("costFunction",observationsSize,varialbeSize,residualSize);

	// get observations
	int LengthObservations = 5;
	double observation_xs[5];
	double observation_ys[5];
	for(int i=0;i<LengthObservations;i++)
	{
		double x = double(i)-2;
		double y = observation2(x);
		observation_xs[i] = x;
		observation_ys[i] = y;
		cout<<"Observation ID "<<i<<", x "<<observation_xs[i]<<", y "<<observation_ys[i]<<endl;
	}

	for(int i=0;i<LengthObservations;i++)
	{
		vector<double> observation_current;
		observation_current.push_back(observation_xs[i]);
		observation_current.push_back(observation_ys[i]);
		costFunction->AddResidualBlock(observation_current);
	}

	//
	cout<<" "<<endl;
	cout<<"alice SetUserInitialization"<<endl;
	sd->SetUserInitialization(costFunction);


	// initialize
	cout<<" "<<endl;
	cout<<"Initialize "<<endl;
	sd->Initialize();
}

void test4()
{
	int observationsSize = 2;
	int residualSize = 1;
	int varialbeSize = 4;
	UserOptimizationManager * manager = new NewtonsOptimizationManager("NewtonsMethod",observationsSize,varialbeSize,residualSize);

	// set variables
	vector<double> variables;
	variables.push_back(1.+101.);
	variables.push_back(2.+101.);
	variables.push_back(3.-101.);
	variables.push_back(4.-101.);
	manager->SetUserInitialization(variables);

	// set cost function
	UserCostFunction* costFunction = new NewtonsCostFunction("costFunction",observationsSize,varialbeSize,residualSize);

	// get observations
	int LengthObservations = 5;
	double observation_xs[5];
	double observation_ys[5];
	for(int i=0;i<LengthObservations;i++)
	{
		double x = double(i)-2;
		double y = observation2(x);
		observation_xs[i] = x;
		observation_ys[i] = y;
		cout<<"Observation ID "<<i<<", x "<<observation_xs[i]<<", y "<<observation_ys[i]<<endl;
	}

	for(int i=0;i<LengthObservations;i++)
	{
		vector<double> observation_current;
		observation_current.push_back(observation_xs[i]);
		observation_current.push_back(observation_ys[i]);
		costFunction->AddResidualBlock(observation_current);
	}

	//
	cout<<" "<<endl;
	cout<<"alice SetUserInitialization"<<endl;
	manager->SetUserInitialization(costFunction);


	// initialize
	cout<<" "<<endl;
	cout<<"Initialize "<<endl;
	manager->Initialize();
}

double observation1(double ob1)
{
	double as[3] = {1,2,3};
	double x = ob1;
	double y = (as[0] + as[1]*x + as[2]*x*x);

	return y;
}

double observation2(double ob1)
{
	double as[4] = {1,2,3,4};
	double x = ob1;
	double y = (as[0] + as[1]*x + as[2]*x*x + as[3]*x*x*x);

	return y;
}
