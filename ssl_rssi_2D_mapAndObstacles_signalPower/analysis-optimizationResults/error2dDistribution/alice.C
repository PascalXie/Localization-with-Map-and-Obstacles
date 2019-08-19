#include <fstream>
#include <iostream>
#include <string>
#include <cmath>

#include "Histogram2D.hh"

using namespace std;

int NumberAnchors = 0;
int NumberNodes = 0;

vector<int> AnchorIDs;
vector<int> NodeIDs;
vector<double> S_anchors, S_nodes;
vector<double> axs, ays, xxs, xys;
vector<double> result_xxs, result_xys;

bool ImportObservation(string filename);
bool ImportOptimizationResult(string filename);

int main()
{
	cout<<"Hello "<<endl;

	//
	ImportObservation("../../data/observations.txt");
	ImportOptimizationResult("../../data/log_OptimizationResults.txt");

	//
	int bin[2] = {10,10};
	double min[2] = {-100, -100};
	double max[2] = {100, 100};
	Histogram2D h1("Error",bin[0], min[0], max[0], bin[1], min[1], max[1]);

	for(int i=0;i<NumberNodes;i++)
	{
		int noID = i;
		double dx = result_xxs[noID] - xxs[noID];
		double dy = result_xys[noID] - xys[noID];
		double error = sqrt(dx*dx + dy*dy);
		h1.Fill(xxs[noID],xys[noID],error);
	}

	// output
	ofstream file("log_distanceError2d.txt");

	for(int i=0;i<bin[0];i++)
	for(int j=0;j<bin[1];j++)
	{
		int xID = i;
		int yID = j;
		double error = h1.GetBinContent(xID, yID);

		double x, y;
		h1.GetBinCentor2D(xID, yID, x, y);

		file<<x<<" "<<y<<" "<<error<<endl;
	}
	file.close();

	return 1;
}

bool ImportOptimizationResult(string filename)
{
	ifstream file(filename.c_str());

	if(file.fail())
	{
		cout<<"Can not find the file \" "<<filename<<" \""<<endl;
		return 0;
	}

	int NodeID = 0;
	double x = 0; 
	double y = 0; 

	while(!file.eof())
	{
		file>>NodeID>>x>>y;

		if(file.eof()) break;

		cout<<"NodeID "<<NodeID<<", Node Position : "<<x<<", "<<y<<endl;

		result_xxs.push_back(x);
		result_xys.push_back(y);
	}

	file.close();

	return 1;
}

bool ImportObservation(string filename)
{
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

	return 1;
}
