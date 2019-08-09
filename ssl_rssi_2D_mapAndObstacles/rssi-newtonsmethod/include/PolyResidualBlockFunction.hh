#ifndef POLYRESIDUALBLOCKFUNCTION_HH
#define POLYRESIDUALBLOCKFUNCTION_HH

#include <iostream>
#include <string>
#include <vector>
#include <cmath>

#include "ToolMapGenerator.hh"
#include "ToolSignalPowerGenerator.hh"

#include "UserResidualBlockFunction.hh"

using namespace std;

class PolyResidualBlockFunction : virtual public UserResidualBlockFunction
{
	public: 
		PolyResidualBlockFunction(string name, vector<double> observations, int SizeObservations, int SizeVariables, int SizeResiduals);
		~PolyResidualBlockFunction();

	public: 
		virtual bool ResidualFunction(vector<double> variables, vector<double> &residuals);
		virtual int  GetObervationsSize();

		//
		// debug
		//
		virtual void ShowResidualFunction();

	public: 
		void SetToolSignalPowerGenerator(ToolSignalPowerGenerator *spg);


	private:
		string name_;
		vector<double> observations_;
		int SizeObservations_;
		int SizeVariables_;
		int SizeResiduals_;


	private:
		ToolMapGenerator *map_;
		ToolSignalPowerGenerator * spg_; 
};
#endif
