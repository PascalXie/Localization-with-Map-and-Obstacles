#ifndef HISTOGRAM2D_HH
#define HISTOGRAM2D_HH

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <cmath>

#include "Histogram.hh"

using namespace std;

class Histogram2D
{
    public:
		Histogram2D(string name, int binSizex, double minx, double maxx, int binSizey, double miny, double maxy);
		~Histogram2D();

    public:
		void Fill(double x, double y, double weight);
		void Show();

    public:
		double GetBinContent(int binIDx, int binIDy);
		bool GetBinCentor2D(int binIDx, int binIDy, double &x, double &y);
		int GetBinX_Border(int currentBin, double &minCurrentBin, double &maxCurrentBin);
		int GetBinXY_Border(int Binx, int Biny, double &minBinx, double &maxBinx, double &minBiny, double &maxBiny);
		int GetBinIDX(double x);
		int GetBinIDY(double y);

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

		vector<Histogram> histogramYs_;
};
#endif
