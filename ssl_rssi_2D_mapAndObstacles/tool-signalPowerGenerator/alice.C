#include <iostream>
#include <vector>
#include <string>
#include <fstream>

#include "ToolNodesGenerator.hh"
#include "ToolMapGenerator.hh"
#include "ToolSignalPowerGenerator.hh"

using namespace std;

vector<double> Nxs_c, Nys_c;
vector<double> Nxs_a_1, Nys_a_1;
vector<double> Nxs_a_2, Nys_a_2;

void NodesGenerator();

int main()
{
	cout<<"Hello "<<endl;

	NodesGenerator();

	ToolMapGenerator * map = new ToolMapGenerator("Map", "3D");

	for(int i=0;i<Nxs_c.size();i++)
	{
		map->SetSample(0.15,Nxs_c[i],Nys_c[i]);
	}
	for(int i=0;i<Nxs_a_1.size();i++)
	{
		map->SetSample(0.,Nxs_a_1[i],Nys_a_1[i]);
	}
	for(int i=0;i<Nxs_a_2.size();i++)
	{
		map->SetSample(0.,Nxs_a_2[i],Nys_a_2[i]);
	}

	map->SetSigma(10);

	//double constant = map->GetConstant(0,0);
	//cout<<"constant "<<constant<<endl;

	// step 2 
	ToolSignalPowerGenerator * spg = new ToolSignalPowerGenerator("spg");
	spg->SetToolMapGenerator(map);

	vector<double> A_anchor, A_node;
	A_anchor.push_back(-100);
	A_anchor.push_back(-100);
	A_anchor.push_back(0);
	A_node.push_back(100);
	A_node.push_back(100);
	A_node.push_back(0);

	double Power_node = 1e4;
	double Power_anchor = spg->GetSignalPower(Power_node,A_anchor,A_node);

	/*
	int bin[3] = {100,100,1};
	double min[3] = {-100,-100,0};
	double max[3] = {100,100,0};
	//map->GetMap(100,-100,100,100,-100,100);
	map->GetMap(bin[0],min[0],max[0],bin[1],min[1],max[1]);
	//map->GetMap(5,-100,100,2,-100,100);

	// write
	ofstream write("log_mapOfConstants_samples.txt");
	write<<bin[0]<<" "<<min[0]<<" "<<max[0]<<" "<<bin[1]<<" "<<min[1]<<" "<<max[1]<<" "<<bin[2]<<" "<<min[2]<<" "<<max[2]<<endl;

	// concrete
	for(int i=0;i<Nxs_c.size();i++)
	{
		double x = Nxs_c[i];
		double y = Nys_c[i];
		double z = 0;
		double constant = 0.15;
		write<<x<<" "<<y<<" "<<z<<" "<<constant<<endl;
	}
	// air
	for(int i=0;i<Nxs_a_1.size();i++)
	{
		double x = Nxs_a_1[i];
		double y = Nys_a_1[i];
		double z = 0;
		double constant = 0.;
		write<<x<<" "<<y<<" "<<z<<" "<<constant<<endl;
	}
	// air 2
	for(int i=0;i<Nxs_a_2.size();i++)
	{
		double x = Nxs_a_2[i];
		double y = Nys_a_2[i];
		double z = 0;
		double constant = 0.;
		write<<x<<" "<<y<<" "<<z<<" "<<constant<<endl;
	}

	write.close();
	*/

	return 1;
}

void NodesGenerator()
{
	vector<double> Nzs;

	// concrete
	ToolNodesGenerator * ng = new ToolNodesGenerator("ng_c", 4000, 1, "2D");
	ng->SetBoundaryX(-50,50);
	ng->SetBoundaryY(-100,100);
	ng->Initialize();

	ng->GetAnchors(Nxs_c, Nys_c, Nzs);

	// air
	ToolNodesGenerator * nga = new ToolNodesGenerator("ng_a1", 2000, 1, "2D");
	nga->SetBoundaryX(-100,-50);
	nga->SetBoundaryY(-100,100);
	nga->Initialize();

	nga->GetAnchors(Nxs_a_1, Nys_a_1, Nzs);


	// air2
	ToolNodesGenerator * nga2 = new ToolNodesGenerator("ng_a2", 2000, 1, "2D");
	nga2->SetBoundaryX(50,100);
	nga2->SetBoundaryY(-100,100);
	nga2->Initialize();

	nga2->GetAnchors(Nxs_a_2, Nys_a_2, Nzs);


	/*
	// debug
	for(int i=0;i<Nxs_c.size();i++)
	{
		cout<<"Concrete ID "<<i<<", x "<<Nxs_c[i]<<", y "<<Nys_c[i]<<endl;
	}
	for(int i=0;i<Nxs_a_1.size();i++)
	{
		cout<<"Air ID "<<i<<", x "<<Nxs_a_1[i]<<", y "<<Nys_a_1[i]<<endl;
	}
	for(int i=0;i<Nxs_a_2.size();i++)
	{
		cout<<"Air ID "<<i<<", x "<<Nxs_a_2[i]<<", y "<<Nys_a_2[i]<<endl;
	}
	*/
}
