#ifndef POLYRESIDUALBLOCKFUNCTION_HH
#define POLYRESIDUALBLOCKFUNCTION_HH

#include <iostream>
#include <string>
#include <vector>
#include <cmath>

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

	private:
		string name_;
		vector<double> observations_;
		int SizeObservations_;
		int SizeVariables_;
		int SizeResiduals_;
};
#endif
