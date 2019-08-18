#ifndef TOOLNODESGENERATOR_HH
#define TOOLNODESGENERATOR_HH

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <cmath>
#include <random>

using namespace std;

class ToolNodesGenerator
{
    public:
		ToolNodesGenerator(string name, int NumberAnchors, int NumberNodes, string MapType);
		~ToolNodesGenerator();

    public:
		void SetBoundaryX(double min, double max);
		void SetBoundaryY(double min, double max);
		void SetBoundaryZ(double min, double max);
		void Initialize();

    public:
		void GetAnchors(vector<double> &As_x, vector<double> &As_y, vector<double> &As_z);
		void GetNodes(vector<double> &Ns_x, vector<double> &Ns_y, vector<double> &Ns_z);

    private:
		bool GenerateRandom(string AxisName);

    private:
		string name_;

		int NumberAnchors_;
		int NumberNodes_;

		string MapType_;
		bool IsMapTypeGood_;
		
		double minX_;
		double maxX_;
		
		double minY_;
		double maxY_;

		double minZ_;
		double maxZ_;

		default_random_engine engine_;

		// anchors 
		vector<double> As_x_;
		vector<double> As_y_;
		vector<double> As_z_;

		// nodes
		vector<double> Ns_x_;
		vector<double> Ns_y_;
		vector<double> Ns_z_;

};
#endif
