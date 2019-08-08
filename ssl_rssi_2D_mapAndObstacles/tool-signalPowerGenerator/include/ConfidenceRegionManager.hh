#ifndef CONFIDENCEREGIONMANAGER_HH
#define CONFIDENCEREGIONMANAGER_HH

#include <iostream>
#include <string>
#include <vector>
#include <cmath>

#include "Histogram2D.hh"

using namespace std;

class ConfidenceRegionManager
{
	public:
		ConfidenceRegionManager(string name);
		~ConfidenceRegionManager();

	public:
		void Initialize2DRegion(int binSizex, double minx, double maxx, int binSizey, double miny, double maxy);
		void SetAnchorAndDistance(double x, double y, double distance);
		void OutputHistogram(string filename);

	public:
		double ComputeProbability(int binIDx, int binIDy);

	private:
		double GaussianFunction(double dis, double mu, double sigma);

	private:
		string name_;

		// X axis
		int    binSizex_;
		double minx_;
		double maxx_;
		double binWidthx_;

		// Y axis
		int    binSizey_;
		double miny_;
		double maxy_;
		double binWidthy_;

		Histogram2D *region2D_;

		// anchors
		vector<double> Anxs_;
		vector<double> Anys_;
		vector<double> Anzs_;
		vector<double> distances_;

		// Gaussian
		double sigma_;
};
#endif
