#include "SteepestOptimizationManager.hh"

//-------------------------
// Constructor
//-------------------------
SteepestOptimizationManager::SteepestOptimizationManager(string name, int ObservationsSize, int VariablesSize, int ResidualsSize)
:	UserOptimizationManager(name+"_baseClass",ObservationsSize,VariablesSize,ResidualsSize),
	name_(name)
{}

//-------------------------
// Destructor
//-------------------------
SteepestOptimizationManager::~SteepestOptimizationManager()
{}

//-------------------------
// Public
//-------------------------
void SteepestOptimizationManager::SetUserInitialization(UserCostFunction* costFunction)
{
	costFunction_ = costFunction;

	IsInitializationCostFunctionSet_ = true;

	cout<<"-----------------------------------------"<<endl;
	cout<<"----"<<endl;
	cout<<"Class SteepestOptimizationManager::SetUserInitialization "<<endl;
	cout<<"Cost Function set, with informations : "<<endl;
	costFunction_->Show();
	cout<<"----"<<endl;
	cout<<"-----------------------------------------"<<endl;
	cout<<endl;
}

void SteepestOptimizationManager::SetUserInitialization(vector<double> variables)
{
	if(variables.size()!=SizeVariables_) 
	{
		cout<<"An error happend with class  SteepestOptimizationManager::SetUserInitialization"<<endl;
		return;
	}

	initializationVariables_ = variables;

	IsInitializationVariablesSet_ = true;

	// debug
	cout<<"-----------------------------------------"<<endl;
	cout<<"----"<<endl;
	cout<<"Class SteepestOptimizationManager::SetUserInitialization "<<endl;
	cout<<"Variables set for initialization, with informations : "<<endl;
	for(int i=0;i<SizeVariables_;i++)
		cout<<"variable ID "<<i<<", value "<<initializationVariables_[i]<<endl;
	cout<<"----"<<endl;
	cout<<"-----------------------------------------"<<endl;
	cout<<endl;
}

void SteepestOptimizationManager::Iteration(vector<double> VariablesPrevious)
{
	//cout<<"Class SteepestOptimizationManager::Iteration"<<endl;
	vector<double> VariablesCurrent;
	VariablesCurrent.clear();

	// step 1 : set variables
	VariablesPrevious_ = VariablesPrevious;

	// step 2 : get derivatives
	vector<double> derivatives;
	bool isDerivativeGood = costFunction_->GradientFunction(VariablesPrevious_,derivatives);

	// step 3 : get alpha step length by line search method
	//          Line search method with 0.618 method
	alpha_stepLength_ = GetAlphaStepLength_LineSearch_0816Method(VariablesPrevious,derivatives);

	// step 4 : get step length
	// step 5 : get current variables
	stepLengths_.clear();
	for(int i=0;i<SizeVariables_;i++)
	{
		int variableID = i;
		double stepLength_currentID = alpha_stepLength_*(-1.)*derivatives[variableID];
		double variable_currentID = VariablesPrevious_[variableID] + stepLength_currentID;
		stepLengths_.push_back(stepLength_currentID);
		VariablesCurrent.push_back(variable_currentID);
	}

	VariablesCurrent_ = VariablesCurrent;


	// debug
	cout<<"----------------------------------------"<<endl;
	cout<<"----"<<endl;
	cout<<"Class SteepestOptimizationManager::Iteration"<<endl;
	cout<<"derivatives of current variables : "<<endl;
	for(int i=0;i<SizeVariables_;i++)
	{
		int variableID = i;
		cout<<"    variableID "<<variableID<<": previous variable "<<VariablesPrevious_[i]<<", derivative "<<derivatives[i]<<", current variable "<<VariablesCurrent_[variableID]<<endl;
	}
	cout<<"----"<<endl;
	cout<<"----------------------------------------"<<endl;

	//
}


void SteepestOptimizationManager::EndOfIteration()
{}
