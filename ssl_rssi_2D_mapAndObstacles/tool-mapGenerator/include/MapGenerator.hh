#ifndef MAPGENERATOR_HH
#define MAPGENERATOR_HH

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <cmath> 
using namespace std;

class MapGenerator
{
    public:
		MapGenerator(string name, string MapType);
		~MapGenerator();

    public:
		void SetSample(double constant, double x, double y, double z=0);
		void SetSigma(double sigma);
		double GetConstant(double x, double y, double z=0);

    public:
		// user's tool
		void GetMap(int binx, double minx, double maxx,int biny, double miny, double maxy,int binz=1, double minz=0, double maxz=0);

    private:
		double GaussianFunction(double mu, double sigma, double value);

    private:
		string name_;
		string MapType_;
		bool IsMapTypeCorrect_;

		// samples
		vector<double> xs_;
		vector<double> ys_;
		vector<double> zs_;
		vector<double> constants_;

		double PI_;

		double sigma_;

};
#endif
