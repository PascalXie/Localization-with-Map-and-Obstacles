#ifndef HISTOGRAM_HH
#define HISTOGRAM_HH

#include <iostream>
#include <string>
#include <vector>
#include <cmath>

using namespace std;

class Histogram
{
    public:
		Histogram(string name, int binSize, double min, double max);
		~Histogram();

    public:
		void Fill(double value, double weight);
		void Show();

    private:
		int GetBin_Border(int currentBin, double &minCurrentBin, double &maxCurrentBin);
		

    private:
		string name_;
		int    binSize_;
		double min_;
		double max_;
		double binWidth_;
		vector<double> counts_;
};

#endif
