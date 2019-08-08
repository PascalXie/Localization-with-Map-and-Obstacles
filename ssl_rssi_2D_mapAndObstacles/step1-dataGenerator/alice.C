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

	// step 1 : generate samples
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

	map->OutputSamples();


	// step 2 : 
	ToolSignalPowerGenerator * spg = new ToolSignalPowerGenerator("spg");
	spg->SetToolMapGenerator(map);

	/*
	vector<double> A_anchor, A_node;
	A_anchor.push_back(-100);
	A_anchor.push_back(-100);
	A_anchor.push_back(0);
	A_node.push_back(100);
	A_node.push_back(100);
	A_node.push_back(0);

	double Power_node = 1e4;
	double Power_anchor = spg->GetSignalPower(Power_node,A_anchor,A_node);
	*/


	// step 3 : generate anchors and nodes 
	int NumberAnchors = 400;
	int NumberNodes = 1;
	ToolNodesGenerator * data = new ToolNodesGenerator("data", NumberAnchors, NumberNodes, "2D");
	data->SetBoundaryX(-100,100);
	data->SetBoundaryY(-100,100);
	data->Initialize();

	vector<double> Data_Ax, Data_Ay, Data_Az;
	vector<double> Data_Nx, Data_Ny, Data_Nz;
	data->GetAnchors(Data_Ax, Data_Ay, Data_Az);
	data->GetNodes(Data_Nx, Data_Ny, Data_Nz);


	ofstream write("observations.txt");
	write<<"NumberAnchors "<<NumberAnchors<<" NumberNodes "<<NumberNodes<<endl;

	for(int j=0;j<Data_Nx.size();j++)
	for(int i=0;i<Data_Ax.size();i++)
	{
		int NodeID = j;
		int AnchorID = i;

		vector<double> A_anchor, A_node;
		A_anchor.push_back(Data_Ax[AnchorID]);
		A_anchor.push_back(Data_Ay[AnchorID]);
		A_anchor.push_back(0);
		A_node.push_back(Data_Nx[NodeID]);
		A_node.push_back(Data_Ny[NodeID]);
		A_node.push_back(0);

		double Power_node = 1e4;
		double Power_anchor = spg->GetSignalPower(Power_node,A_anchor,A_node);

		write<<AnchorID<<" "<<A_anchor[0]<<" "<<A_anchor[1]<<" "<<A_anchor[2]<<" "<<Power_anchor<<" ";
		write<<NodeID<<" "<<A_node[0]<<" "<<A_node[1]<<" "<<A_node[2]<<" "<<Power_node<<endl;
	}

	write.close();


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
