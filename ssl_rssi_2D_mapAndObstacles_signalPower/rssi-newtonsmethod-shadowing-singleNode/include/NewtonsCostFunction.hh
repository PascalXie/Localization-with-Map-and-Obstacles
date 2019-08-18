#ifndef NEWTONSCOSTFUNCTION_HH
#define NEWTONSCOSTFUNCTION_HH

#include <iostream>
#include <string>
#include <vector>
#include <cmath>

#include "UserCostFunction.hh"

#include "UserResidualBlockFunction.hh"
#include "PolyResidualBlockFunction.hh"

using namespace std;

class NewtonsCostFunction : virtual public UserCostFunction
{
    public:
		NewtonsCostFunction(string name, int SizeObservations, int SizeVariables, int SizeResiduals);
		~NewtonsCostFunction();

    public:
		void SetStepLength(double delta);
		void Show();

    public:
		virtual void AddResidualBlock(vector<double> observations);
		virtual bool CostFunction(vector<double> variables, vector<double> &CostFunctionValues);
		virtual bool GetOneDerivative(int VarialbleID, vector<double> variables, double &theDerivativeValue);
		virtual bool GetOneSecondOrderDerivative(int FirstPartialDerivativeVarialbleID, int SecondPartialDerivativeVarialbleID, vector<double> variables, double &theDerivativeValue);

	protected:
		bool GetSecondOrderDerivative_Part1(int ResidualBlockID, int FirstPartialDerivativeVarialbleID, int SecondPartialDerivativeVarialbleID, vector<double> variables, double &theDerivativeValue);
		bool GetSecondOrderDerivative_Part2(int ResidualBlockID, int FirstPartialDerivativeVarialbleID, int SecondPartialDerivativeVarialbleID, vector<double> variables, double &theDerivativeValue);

	protected:
		// for derivative calculation
		double delta_;

    private:
		string name_;
};
#endif
