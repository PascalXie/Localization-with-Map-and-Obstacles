#include "NewtonsCostFunction.hh"

//-------------------------
// Constructor
//-------------------------
NewtonsCostFunction::NewtonsCostFunction(string name, int SizeObservations, int SizeVariables, int SizeResiduals)
:	UserCostFunction(name+"_SuperType",SizeObservations,SizeVariables,SizeResiduals),
	name_(name),
	delta_(1e-6)
{
	//cout<<"NewtonsCostFunction "<<name_<<endl;
}

//-------------------------
// Destructor
//-------------------------
NewtonsCostFunction::~NewtonsCostFunction()
{}

//-------------------------
// Public
//-------------------------
void NewtonsCostFunction::AddResidualBlock(vector<double> observations)
{
	if(observations.size()!=SizeObservations_) 
	{
		cout<<"Wrong happend in class NewtonsCostFunction::AddResidualBlock."<<endl;
		return;
	}

	int ResidualID = ResidualBlockFunctions_.size() + 1;
	string name_residual = name_+"_residual_" + to_string(ResidualID);
	UserResidualBlockFunction* res_current = new PolyResidualBlockFunction(name_residual,observations,SizeObservations_,SizeVariables_,SizeResiduals_);

	ResidualBlockFunctions_.push_back(res_current);

	/*
	// debug
	cout<<"Debug class NewtonsCostFunction::AddResidualBlock"<<endl;
	cout<<"ResidualBlockFunctions_ size "<<ResidualBlockFunctions_.size()<<endl;
	*/
}

bool NewtonsCostFunction::CostFunction(vector<double> variables, vector<double> &CostFunctionValues)
{
	if(variables.size()!=SizeVariables_)
	{
		cout<<"An Error happend in Class NewtonsCostFunction::CostFunction, Variables size is NOT correct"<<endl;
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
		cout<<"debug NewtonsCostFunction::CostFunction ResidualID "<<ResidualID<<", residuals_current size "<<residuals_current.size()<<", residuals_current "<<residuals_current[0]<<endl;
		*/

		// get cost function value squared
		// costFunctionSquared_currentDimension = sum(residual_currentDiemsntion^2)
		for(int i=0;i<SizeResiduals_;i++)
		{
			int ResidualDimensionID = i;
			double single_residual_current = residuals_current[ResidualDimensionID];
			double single_costFunctionValue_current = single_residual_current*single_residual_current;
			//double single_costFunctionValue_current = 1.*single_residual_current;
			//double single_costFunctionValue_current = sqrt(single_residual_current*single_residual_current);
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


	/*
	// For inequality constrained optimizaiton 
	// interior point method is used 
	// with Barrior function
	double t = 1.;
	double xx = variables[0];
	double xy = variables[1];
	for(int i=0;i<SizeResiduals_;i++)
	{
		int ResidualDimensionID = i;
		CostFunctionValues_current[ResidualDimensionID] = CostFunctionValues_current[ResidualDimensionID] - 1./t*log(-1.*xx+100.);
		CostFunctionValues_current[ResidualDimensionID] = CostFunctionValues_current[ResidualDimensionID] - 1./t*log( 1.*xx+100.);
		CostFunctionValues_current[ResidualDimensionID] = CostFunctionValues_current[ResidualDimensionID] - 1./t*log(-1.*xy+100.);
		CostFunctionValues_current[ResidualDimensionID] = CostFunctionValues_current[ResidualDimensionID] - 1./t*log( 1.*xy+100.);
	}
	*/
	

	CostFunctionValues = CostFunctionValues_current;

	return true;
}

bool NewtonsCostFunction::GetOneDerivative(int VarialbleID, vector<double> variables, double &theDerivativeValue)
{
	// derivatives for the one variable asked

	// debug
	if(VarialbleID>=SizeVariables_)
	{
		cout<<"Error happend in Class NewtonsCostFunction::GetOneDerivative "<<endl;
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
		cout<<"Error happend in Class NewtonsCostFunction::GetOneDerivative "<<endl;
		cout<<"SizeCostFunction must be 1 "<<endl;
		return false;
	}

	theDerivativeValue = (CostFunctionValues_right[0] - CostFunctionValues_left[0])/delta_;

	/*
	// debug
	cout<<"Class NewtonsCostFunction::GetOneDerivative CostFunctionValues_right[0] "<<CostFunctionValues_right[0]<<endl;
	cout<<"Class NewtonsCostFunction::GetOneDerivative CostFunctionValues_left[0] "<<CostFunctionValues_left[0]<<endl;
	cout<<"Class NewtonsCostFunction::GetOneDerivative Derivative "<<theDerivativeValue<<endl;
	*/
 
	return true;
}

bool NewtonsCostFunction::GetOneSecondOrderDerivative(int FirstPartialDerivativeVarialbleID, int SecondPartialDerivativeVarialbleID, vector<double> variables, double &theDerivativeValue)
{
	// second order derivatives are used for the Newton's method 

	// checking
	if(FirstPartialDerivativeVarialbleID>=SizeVariables_||SecondPartialDerivativeVarialbleID>=SizeVariables_)
	{
		cout<<"An Error happend in Class NewtonsCostFunction::GetOneSecondOrderDerivative"<<endl;
		return false;
	}

	// step 0 : set step length - delta
	double delta_FirstVar = delta_;
	double delta_SecondVar = delta_;

	// step 1 : calculate
	double theSecondOrderDerivativeValue = 0;

	for(int i=0;i<ResidualBlockFunctions_.size();i++)
	{
		int ResidualBlockID = i;

		// part1
		double part1_value = 666;
		bool isPart1Good = GetSecondOrderDerivative_Part1(ResidualBlockID,FirstPartialDerivativeVarialbleID,SecondPartialDerivativeVarialbleID,variables,part1_value);

		// part2
		double part2_value = 666;
		bool isPart2Good = GetSecondOrderDerivative_Part2(ResidualBlockID,FirstPartialDerivativeVarialbleID,SecondPartialDerivativeVarialbleID,variables,part2_value);

		if(isPart1Good==false||isPart2Good==false) 
		{
			cout<<"An Error happend in Class NewtonsCostFunction::GetOneSecondOrderDerivative (2)"<<endl;
			return false;
		}

		theSecondOrderDerivativeValue = theSecondOrderDerivativeValue + (part1_value+part2_value);

		/*
		// debug
		cout<<"part1_value "<<part1_value<<endl;
		cout<<"part2_value "<<part2_value<<endl;
		cout<<"theSecondOrderDerivativeValue "<<theSecondOrderDerivativeValue<<endl;
		*/
	}

	theDerivativeValue = theSecondOrderDerivativeValue;
	
	/*
	// debug
	cout<<"theSecondOrderDerivativeValue "<<theSecondOrderDerivativeValue<<endl;
	*/

	return true;
}

bool NewtonsCostFunction::GetSecondOrderDerivative_Part1(int ResidualBlockID, int FirstPartialDerivativeVarialbleID, int SecondPartialDerivativeVarialbleID, vector<double> variables, double &theDerivativeValue)
{
	if(variables.size()!=SizeVariables_)
	{
		cout<<"An Error happend in class NewtonsCostFunction::GetSecondOrderDerivative_Part1"<<endl;
		return false;
	}

	// Hessian Matrix Element
	// part1_1
	vector<double> varialbes_part1_1_1 = variables;
	varialbes_part1_1_1[FirstPartialDerivativeVarialbleID] += delta_;

	vector<double> varialbes_part1_1_2 = variables;
	varialbes_part1_1_2[FirstPartialDerivativeVarialbleID] -= delta_;

	vector<double> residuals_part1_1_1; 
	vector<double> residuals_part1_1_2; 
	bool isPart1_1_1Good = ResidualBlockFunctions_[ResidualBlockID]->ResidualFunction(varialbes_part1_1_1,residuals_part1_1_1);
	bool isPart1_1_2Good = ResidualBlockFunctions_[ResidualBlockID]->ResidualFunction(varialbes_part1_1_2,residuals_part1_1_2);


	// part1_2
	vector<double> varialbes_part1_2_1 = variables;
	vector<double> varialbes_part1_2_2 = variables;
	varialbes_part1_2_1[SecondPartialDerivativeVarialbleID] += delta_;
	varialbes_part1_2_2[SecondPartialDerivativeVarialbleID] -= delta_;

	vector<double> residuals_part1_2_1; 
	vector<double> residuals_part1_2_2; 
	bool isPart1_2_1Good = ResidualBlockFunctions_[ResidualBlockID]->ResidualFunction(varialbes_part1_2_1,residuals_part1_2_1);
	bool isPart1_2_2Good = ResidualBlockFunctions_[ResidualBlockID]->ResidualFunction(varialbes_part1_2_2,residuals_part1_2_2);

	// calculate
	double value = (residuals_part1_1_1[0]-residuals_part1_1_2[0])/(2.*delta_) * (residuals_part1_2_1[0]-residuals_part1_2_2[0])/(2.*delta_);

	theDerivativeValue = value;

	/*
	// debug
	cout<<"varialbes_part1_1_1 "<<varialbes_part1_1_1[0]<<", "<<varialbes_part1_1_1[1]<<", "<<varialbes_part1_1_1[2]<<endl;
	cout<<"varialbes_part1_1_2 "<<varialbes_part1_1_2[0]<<", "<<varialbes_part1_1_2[1]<<", "<<varialbes_part1_1_2[2]<<endl;
	cout<<"varialbes_part1_2_1 "<<varialbes_part1_2_1[0]<<", "<<varialbes_part1_2_1[1]<<", "<<varialbes_part1_2_1[2]<<endl;
	cout<<"varialbes_part1_2_2 "<<varialbes_part1_2_2[0]<<", "<<varialbes_part1_2_2[1]<<", "<<varialbes_part1_2_2[2]<<endl;

	cout<<"residuals_part1_1_1 "<<residuals_part1_1_1[0]<<endl;
	cout<<"residuals_part1_1_2 "<<residuals_part1_1_2[0]<<endl;
	cout<<"residuals_part1_2_1 "<<residuals_part1_2_1[0]<<endl;
	cout<<"residuals_part1_2_2 "<<residuals_part1_2_2[0]<<endl;

	cout<<"debug NewtonsCostFunction::GetSecondOrderDerivative_Part1 theDerivativeValue : "<<theDerivativeValue<<endl;
	*/

	return true;
}

bool NewtonsCostFunction::GetSecondOrderDerivative_Part2(int ResidualBlockID, int FirstPartialDerivativeVarialbleID, int SecondPartialDerivativeVarialbleID, vector<double> variables, double &theDerivativeValue)
{
	if(variables.size()!=SizeVariables_)
	{
		cout<<"An Error happend in class NewtonsCostFunction::GetSecondOrderDerivative_Part1"<<endl;
		return false;
	}

	// Hessian Matrix Element
	vector<double> varialbes_part2_1 = variables;
	vector<double> varialbes_part2_2 = variables;
	vector<double> varialbes_part2_3 = variables;
	vector<double> varialbes_part2_4 = variables;

	varialbes_part2_1[FirstPartialDerivativeVarialbleID]  += delta_;
	varialbes_part2_1[SecondPartialDerivativeVarialbleID] += delta_;

	varialbes_part2_2[FirstPartialDerivativeVarialbleID]  -= delta_;
	varialbes_part2_2[SecondPartialDerivativeVarialbleID] += delta_;

	varialbes_part2_3[FirstPartialDerivativeVarialbleID]  += delta_;
	varialbes_part2_3[SecondPartialDerivativeVarialbleID] -= delta_;

	varialbes_part2_4[FirstPartialDerivativeVarialbleID]  -= delta_;
	varialbes_part2_4[SecondPartialDerivativeVarialbleID] -= delta_;

	vector<double> residuals_part2_1; 
	vector<double> residuals_part2_2; 
	vector<double> residuals_part2_3; 
	vector<double> residuals_part2_4; 

	bool isPart2_1Good = ResidualBlockFunctions_[ResidualBlockID]->ResidualFunction(varialbes_part2_1,residuals_part2_1);
	bool isPart2_2Good = ResidualBlockFunctions_[ResidualBlockID]->ResidualFunction(varialbes_part2_2,residuals_part2_2);
	bool isPart2_3Good = ResidualBlockFunctions_[ResidualBlockID]->ResidualFunction(varialbes_part2_3,residuals_part2_3);
	bool isPart2_4Good = ResidualBlockFunctions_[ResidualBlockID]->ResidualFunction(varialbes_part2_4,residuals_part2_4);


	vector<double> residual_origin;
	bool isOriginGood = ResidualBlockFunctions_[ResidualBlockID]->ResidualFunction(variables,residual_origin);


	double value = residual_origin[0]/(4.*delta_*delta_)*(residuals_part2_1[0]-residuals_part2_2[0]-residuals_part2_3[0]+residuals_part2_4[0]);
	theDerivativeValue = value;

	/*
	// debug
	ResidualBlockFunctions_[ResidualBlockID]->ShowResidualFunction();
	cout<<"variables         : "<<variables[0]<<", "<<variables[1]<<", "<<variables[2]<<endl;
	cout<<"varialbes_part2_1 : "<<varialbes_part2_1[0]<<", "<<varialbes_part2_1[1]<<", "<<varialbes_part2_1[2]<<endl;
	cout<<"varialbes_part2_2 : "<<varialbes_part2_2[0]<<", "<<varialbes_part2_2[1]<<", "<<varialbes_part2_2[2]<<endl;
	cout<<"varialbes_part2_3 : "<<varialbes_part2_3[0]<<", "<<varialbes_part2_3[1]<<", "<<varialbes_part2_3[2]<<endl;
	cout<<"varialbes_part2_4 : "<<varialbes_part2_4[0]<<", "<<varialbes_part2_4[1]<<", "<<varialbes_part2_4[2]<<endl;

	cout<<"residuals_original : "<<residual_origin[0]<<endl;
	cout<<"residuals_part2_1 : "<<residuals_part2_1[0]<<endl;
	cout<<"residuals_part2_2 : "<<residuals_part2_2[0]<<endl;
	cout<<"residuals_part2_3 : "<<residuals_part2_3[0]<<endl;
	cout<<"residuals_part2_4 : "<<residuals_part2_4[0]<<endl;

	cout<<"theDerivativeValue "<<theDerivativeValue<<endl;
	*/

	return true;
}

//-------------------------
// Public: derivative
//-------------------------
void NewtonsCostFunction::SetStepLength(double delta)
{
	delta_ = delta;
}

//-------------------------
// Public: debug
//-------------------------
void NewtonsCostFunction::Show()
{
	cout<<"class NewtonsCostFunction : show name "<<name_<<endl;

	for(int i=0;i<ResidualBlockFunctions_.size();i++)
	{
		int ResidualID = i;
		cout<<"ResidualID "<<ResidualID<<": "<<endl;
		ResidualBlockFunctions_[ResidualID]->ShowResidualFunction();
	}
}
