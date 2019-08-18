#ifndef TOOLSIGNALPOWERGENERATOR_HH
#define TOOLSIGNALPOWERGENERATOR_HH

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <cmath> 

#include "Units.hh"

#include "ToolMapGenerator.hh"

using namespace std;

class ToolSignalPowerGenerator
{
    public:
		ToolSignalPowerGenerator(string name);
		~ToolSignalPowerGenerator();

    public:
		//  ToolMapGenerator
		void SetToolMapGenerator(ToolMapGenerator *map);

		// compute
		double GetSignalPowerStrength_dBm(double S_node, vector<double> A_anchor, vector<double> A_node);

		// parameters
		void SetFactor(double C);

    private:
		string name_;

		//  ToolMapGenerator
		ToolMapGenerator * map_;
		bool isMapGood_;

		// Integral
		int binSize_;

		// Factor
		double E_;
		double C_;
};
#endif
