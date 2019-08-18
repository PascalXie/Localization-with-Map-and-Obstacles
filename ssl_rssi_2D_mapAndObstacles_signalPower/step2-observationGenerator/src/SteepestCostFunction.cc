#include "SteepestCostFunction.hh"

//-------------------------
// Constructor
//-------------------------
SteepestCostFunction::SteepestCostFunction(string name, int SizeObservations, int SizeVariables, int SizeResiduals)
:	UserCostFunction(name+"_SuperType",SizeObservations,SizeVariables,SizeResiduals),
	name_(name),
	delta_(1e-6)
{
	//cout<<"SteepestCostFunction "<<name_<<endl;
}

//-------------------------
// Destructor
//-------------------------
SteepestCostFunction::~SteepestCostFunction()
{}

//-------------------------
// Public
//-------------------------
void SteepestCostFunction::AddResidualBlock(vector<double> observations)
{
	if(observations.size()!=SizeObservations_) 
	{
		cout<<"Wrong happend in class SteepestCostFunction::AddResidualBlock."<<endl;
		return;
	}

	int ResidualID = ResidualBlockFunctions_.size() + 1;
	string name_residual = name_+"_residual_" + to_string(ResidualID);
	UserResidualBlockFunction* res_current = new PolyResidualBlockFunction(name_residual,observations,SizeObservations_,SizeVariables_,SizeResiduals_);

	ResidualBlockFunctions_.push_back(res_current);

	/*
	// debug
	cout<<"Debug class SteepestCostFunction::AddResidualBlock"<<endl;
	cout<<"ResidualBlockFunctions_ size "<<ResidualBlockFunctions_.size()<<endl;
	*/
}

bool SteepestCostFunction::CostFunction(vector<double> variables, vector<double> &CostFunctionValues)
{
	if(variables.size()!=SizeVariables_)
	{
		cout<<"An Error happend in Class SteepestCostFunction::CostFunction, Variables size is NOT correct"<<endl;
		return false;
	}

	/*
	// debug
	cout<<"variables : "<<variables[0]<<", "<<variables[1]<<", "<<variables[2]<<endl;
	*/

	// initiate
	vector<double> CostFunctionValues_current;
	vector<double> CostFunctionValues_squared_current;
	for(int i=0;i<SizeResiduals_;i++)
	{
		CostFunctionValues_current.push_back(0);
		CostFunctionValues_squared_current.push_back(0);
	}

	for(int i=0;i<ResidualBlockFunctions_.size();i++)
	{
		int ResidualID = i;

		// get residual from current residualFunction
		vector<double> residuals_current;
		bool isResidualGood = ResidualBlockFunctions_[ResidualID]->ResidualFunction(variables,residuals_current);

		/*
		// debug
		cout<<"debug ResidualID "<<ResidualID<<", residuals_current size "<<residuals_current.size()<<", residuals_current "<<residuals_current[0]<<endl;
		*/

		// get cost function value squared
		// costFunctionSquared_currentDimension = sum(residual_currentDiemsntion^2)
		for(int i=0;i<SizeResiduals_;i++)
		{
			int ResidualDimensionID = i;
			double single_residual_current = residuals_current[ResidualDimensionID];
			double single_costFunctionValue_current = single_residual_current*single_residual_current;
			CostFunctionValues_squared_current[ResidualDimensionID] += single_costFunctionValue_current;
		}
	}

	// get cost function  
	// costFunction_currentDimension = root mean square of  costFunctionSquared_currentDimension
	for(int i=0;i<SizeResiduals_;i++)
	{
		int ResidualDimensionID = i;
		CostFunctionValues_current[ResidualDimensionID] = CostFunctionValues_squared_current[ResidualDimensionID];
	}

	CostFunctionValues = CostFunctionValues_current;

	return true;
}

bool SteepestCostFunction::GetOneDerivative(int VarialbleID, vector<double> variables, double &theDerivativeValue)
{
	// derivatives for the one variable asked

	// debug
	if(VarialbleID>=SizeVariables_)
	{
		cout<<"Error happend in Class SteepestCostFunction::GetOneDerivative "<<endl;
		return false;
	}

	// calculate
	// derivative = (CostFunctionValues_right - CostFunctionValues_left)/delta

	vector<double> CostFunctionValues_left;
	vector<double> variables_left = variables;
	bool isCostFunctionGood_left = CostFunction(variables_left,CostFunctionValues_left);

	vector<double> CostFunctionValues_right;
	vector<double> variables_right = variables;
	variables_right[VarialbleID] += delta_;
	bool isCostFunctionGood_right = CostFunction(variables_right,CostFunctionValues_right);

	if(CostFunctionValues_right.size()!=1)
	{
		cout<<"Error happend in Class SteepestCostFunction::GetOneDerivative "<<endl;
		cout<<"SizeCostFunction must be 1 "<<endl;
		return false;
	}

	theDerivativeValue = (CostFunctionValues_right[0] - CostFunctionValues_left[0])/delta_;

	/*
	// debug
	cout<<"Class SteepestCostFunction::GetOneDerivative CostFunctionValues_right[0] "<<CostFunctionValues_right[0]<<endl;
	cout<<"Class SteepestCostFunction::GetOneDerivative CostFunctionValues_left[0] "<<CostFunctionValues_left[0]<<endl;
	cout<<"Class SteepestCostFunction::GetOneDerivative Derivative "<<theDerivativeValue<<endl;
	*/
 
	return true;
}

bool SteepestCostFunction::GetOneSecondOrderDerivative(int FirstPartialDerivativeVarialbleID, int SecondPartialDerivativeVarialbleID, vector<double> variables, double &theDerivativeValue)
{
	// do nothing 
	// second order derivatives are NOT used for the steepest descent method 
	return false;
}

//-------------------------
// Public: derivative
//-------------------------
void SteepestCostFunction::SetStepLength(double delta)
{
	delta_ = delta;
}

//-------------------------
// Public: debug
//-------------------------
void SteepestCostFunction::Show()
{
	cout<<"class SteepestCostFunction : show name "<<name_<<endl;

	for(int i=0;i<ResidualBlockFunctions_.size();i++)
	{
		int ResidualID = i;
		cout<<"ResidualID "<<ResidualID<<": "<<endl;
		ResidualBlockFunctions_[ResidualID]->ShowResidualFunction();
	}
}
