#include "UserOptimizationManager.hh"

//-------------------------
// Constructor
//-------------------------
UserOptimizationManager::UserOptimizationManager(string name, int SizeObservations, int SizeVariables, int SizeResiduals)
:	name_(name),
	SizeObservations_(SizeObservations),
	SizeVariables_(SizeVariables),
	SizeResiduals_(SizeResiduals),
	costFunction_(NULL),
	IsInitializationCostFunctionSet_(false),
	IsInitializationVariablesSet_(false),
	alpha_stepLength_(1e-1),
	TotalNumberIterations_(0),
	MaximumNumberIterations_(1e4),
	UserReferencedLength_(-1),
	UserEpsilonForTerminating_(1e-3)
{}

//-------------------------
// Destructor
//-------------------------
UserOptimizationManager::~UserOptimizationManager()
{}


//-------------------------
// Public  
//-------------------------
void UserOptimizationManager::Initialize()
{
	// step 1 : set variables for initialization
	VariablesCurrent_ = initializationVariables_;

	// step 2 : loop
	for(int i=0;i<MaximumNumberIterations_;i++)
	{
		vector<double> VariablesPrevious = VariablesCurrent_;
		Iteration(VariablesPrevious);

		// get errors
		vector<double> errors_costFunction;
		vector<double> costFunction_previous;
		vector<double> costFunction_current;
		double error_squaredSum = 0;
		bool isCostFuntionGood1 = costFunction_->CostFunction(VariablesPrevious_,costFunction_previous);
		bool isCostFuntionGood2 = costFunction_->CostFunction(VariablesCurrent_,costFunction_current);
		for(int i=0;i<SizeResiduals_;i++)
		{
			double error_current = costFunction_current[i] - costFunction_previous[i];
			errors_costFunction.push_back(error_current);
			error_squaredSum = error_squaredSum + error_current*error_current;
		}

		TotalNumberIterations_++;

		/*
		// debug
		cout<<"----------------------------------------"<<endl;
		cout<<"----"<<endl;
		cout<<"Class UserOptimizationManager::Initialize"<<endl;
		cout<<"Iteration Loops ID (start with 1) "<<TotalNumberIterations_<<", error : "<<endl;
		for(int errorID=0;errorID<SizeResiduals_;errorID++)
		{
			//cout<<"    ErrorID "<<errorID<<": error "<<errors_costFunction[errorID]<<endl;
			cout<<"    ErrorID "<<errorID<<": error "<<error_squaredSum<<endl;
		}
		cout<<"UserEpsilonForTerminating_ : "<<UserEpsilonForTerminating_<<endl;
		cout<<"----"<<endl;
		cout<<"----------------------------------------"<<endl;
		*/

		// terminate
		if(error_squaredSum<UserEpsilonForTerminating_) break;

		// Users function
		EndOfIteration();
	}
}

void UserOptimizationManager::SetAlphaStepLength(double alpha_stepLength)
{
	alpha_stepLength_ = alpha_stepLength;
}

void UserOptimizationManager::GetVariables(vector<double> &Variables)
{
	Variables.clear();
	Variables = VariablesCurrent_;
}
//-------------------------
// Protected : Step length computing
//-------------------------
double UserOptimizationManager::GetAlphaStepLength_LineSearch_0816Method(vector<double> Variables, vector<double> DescentDirection)
{
	// step length  varies from (0) to (alpha_stepLength_*DescentDirection)
	// that makes alpha_stepLength_ varise from 0 to 1

	double alpha_lower = 0;
	double alpha_upper = 1;

	double tau = 0.816;

	double alpha_lower_temp = 0;
	double alpha_upper_temp = 1;

	//// the first Iteration
	//alpha_lower_temp = alpha_lower + (1.-tau)*(alpha_upper-alpha_lower);
	//alpha_upper_temp = tau * (alpha_upper-alpha_lower);

	// iterations lefted
	for(int i=0;i<1e2;i++)
	{
		int iterationID = i;

		alpha_lower_temp = alpha_lower + (1.-tau)*(alpha_upper-alpha_lower);
		alpha_upper_temp = tau * (alpha_upper-alpha_lower);

		// get value of Cost function for lower_temp : (Variables+alpha_lower_temp*DescentDirection)
		// get value of Cost function for upper_temp : (Variables+alpha_lower_temp*DescentDirection)
		vector<double> CostFunctionValue_alphaLowerTemp;
		vector<double> CostFunctionValue_alphaUpperTemp;
		vector<double> variables_lower = Variables;
		vector<double> variables_upper = Variables;
		for(int j=0;j<SizeVariables_;j++)
		{
			int variableID = j;
			variables_lower[variableID] = Variables[variableID] + alpha_lower_temp*DescentDirection[variableID];
			variables_upper[variableID] = Variables[variableID] + alpha_upper_temp*DescentDirection[variableID];
		}

		bool isLowerGood = costFunction_->CostFunction(variables_lower,CostFunctionValue_alphaLowerTemp); 
		bool isUpperGood = costFunction_->CostFunction(variables_upper,CostFunctionValue_alphaUpperTemp); 

		// compairing
		double CostFunctionValue_Sum_lower = 0;
		double CostFunctionValue_Sum_upper = 0;
		for(int j=0;j<SizeResiduals_;j++)
		{
			int ResidualID = j;
			CostFunctionValue_Sum_lower += CostFunctionValue_alphaLowerTemp[ResidualID];
			CostFunctionValue_Sum_upper += CostFunctionValue_alphaUpperTemp[ResidualID];
		}

		if(CostFunctionValue_Sum_lower<=CostFunctionValue_Sum_upper)
		{
			alpha_upper = alpha_upper_temp;
		}
		if(CostFunctionValue_Sum_lower>CostFunctionValue_Sum_upper)
		{
			alpha_lower = alpha_lower_temp;
		}

		/*
		// debug
		cout<<"Debug UserOptimizationManager::GetAlphaStepLength_LineSearch_0816Method : alpha_lower "<<alpha_lower<<", alpha_upper "<<alpha_upper<<endl;
		*/

		// iteration over
		if(alpha_upper-alpha_lower<0.01) break;
	}

	double alpha = (alpha_upper+alpha_lower)/2.;

	return alpha;
}

//-------------------------
// Protected : User settings
//-------------------------
// for step length computing
void UserOptimizationManager::SetUserReferencedLength(double UserReferencedLength)
{
	UserReferencedLength_ = UserReferencedLength;
}

// for teriminating the loop
void UserOptimizationManager::SetUserEpsilonForTerminating(double UserEpsilonForTerminating)
{
	UserEpsilonForTerminating_ = UserEpsilonForTerminating;
}
