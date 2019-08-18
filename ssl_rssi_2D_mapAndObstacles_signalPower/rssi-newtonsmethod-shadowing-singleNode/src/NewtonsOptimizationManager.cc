#include "NewtonsOptimizationManager.hh"

//-------------------------
// Constructor
//-------------------------
NewtonsOptimizationManager::NewtonsOptimizationManager(string name, int ObservationsSize, int VariablesSize, int ResidualsSize)
:	UserOptimizationManager(name+"_baseClass",ObservationsSize,VariablesSize,ResidualsSize),
	name_(name),
	JacobianMatrix_(VariablesSize,1),
	HessianMatrix_(VariablesSize,VariablesSize)
{

	// output data
	write = new ofstream("log_nodeResults_"+name_+".txt");
}

//-------------------------
// Destructor
//-------------------------
NewtonsOptimizationManager::~NewtonsOptimizationManager()
{}

//-------------------------
// Public
//-------------------------
void NewtonsOptimizationManager::SetUserInitialization(UserCostFunction* costFunction)
{
	costFunction_ = costFunction;

	IsInitializationCostFunctionSet_ = true;

	/*
	cout<<"-----------------------------------------"<<endl;
	cout<<"----"<<endl;
	cout<<"Class NewtonsOptimizationManager::SetUserInitialization "<<endl;
	cout<<"Cost Function set, with informations : "<<endl;
	costFunction_->Show();
	cout<<"----"<<endl;
	cout<<"-----------------------------------------"<<endl;
	cout<<endl;
	*/
}

void NewtonsOptimizationManager::SetUserInitialization(vector<double> variables)
{
	if(variables.size()!=SizeVariables_) 
	{
		cout<<"An error happend with class  NewtonsOptimizationManager::SetUserInitialization"<<endl;
		return;
	}

	initializationVariables_ = variables;

	IsInitializationVariablesSet_ = true;

	/*
	// debug
	cout<<"-----------------------------------------"<<endl;
	cout<<"----"<<endl;
	cout<<"Class NewtonsOptimizationManager::SetUserInitialization "<<endl;
	cout<<"Variables set for initialization, with informations : "<<endl;
	for(int i=0;i<SizeVariables_;i++)
		cout<<"variable ID "<<i<<", value "<<initializationVariables_[i]<<endl;
	cout<<"----"<<endl;
	cout<<"-----------------------------------------"<<endl;
	cout<<endl;
	*/
}

void NewtonsOptimizationManager::Iteration(vector<double> VariablesPrevious)
{
	vector<double> VariablesCurrent;
	VariablesCurrent.clear();

	// step 1 : set variables
	VariablesPrevious_ = VariablesPrevious;

	// step 2.1 : get Jacobian Matrix, i.e. Gradient, the frist order derivatives of Cost function
	vector<double> Jacobian;
	bool isJacobianGood = costFunction_->GradientFunction(VariablesPrevious_,Jacobian);

	if(!isJacobianGood)
	{
		cout<<"An Error happend in NewtonsOptimizationManager::Iteration with Jacobian"<<endl;
		return;
	}

	for(int i=0;i<SizeVariables_;i++)
	{
		int rowID = i;
		JacobianMatrix_(rowID,0) = Jacobian[rowID];
	}

	// step 2.2 : get Hessian Matrix, which is the second order derivatives of Cost function
	vector<double> Hessian;
	bool isHessianGood = costFunction_->HessianMatrixFunction(VariablesPrevious_,Hessian);
	if(!isHessianGood)
	{
		cout<<"An Error happend in NewtonsOptimizationManager::Iteration with Hessian"<<endl;
		return;
	}

	//  step 2.2 : Fill HessianMatrix
	for(int i=0;i<SizeVariables_;i++)
	for(int j=0;j<SizeVariables_;j++)
	{
		if(j<i) continue;
		if(j==i)
		{
			int rowID = i;
			int columnID = j;
			int ID = rowID*SizeVariables_+columnID;
			HessianMatrix_(rowID,columnID) = Hessian[ID];
			continue;
		}

		int rowID = i;
		int columnID = j;
		int ID = rowID*SizeVariables_+columnID;
		HessianMatrix_(rowID,columnID) = Hessian[ID];
		HessianMatrix_(columnID,rowID) = Hessian[ID];
	}


	// step 3 : choose Newton's method or steepest descent method
	// if Hessian Matrix is positive definite, choose Newton's methos
	// if Hessian Matrix is negative definite, choose Steepteset descent method 
	
	SelfAdjointEigenSolver<MatrixXd> eigenSolver(HessianMatrix_);

	bool areAllEigenValuesPositive = true;
	for(int i=0;i<SizeVariables_;i++)
	{
		double lambda = eigenSolver.eigenvalues()[i];
		if(lambda<=1e-5) areAllEigenValuesPositive=false;
	}

	//step 3 : make descent direction ready
	MatrixXd DescentDirection(SizeVariables_,1);

	if(areAllEigenValuesPositive) 
	{
		// The Hessian matrix is positive definite, the Newton's method is going to be uesd
		// get inverse matrix of Hessian
		// get descent direction : DescentDirection
		DescentDirection = -1.*HessianMatrix_.inverse()*JacobianMatrix_;

	}
	else
	{
		// The Hessian matrix is not positive definite, the steepest descent method is going to be uesd
		DescentDirection = -1.*JacobianMatrix_;
	}

	// step 4 : get alpha step length by line search method
	//          Line search method with 0.618 method
	// step 4.1 : decrease the descent direction if the norm of it is too large
	if(UserReferencedLength_>0)
	{
		double norm_current = DescentDirection.norm();
		if(norm_current>UserReferencedLength_)
		{
			// debug
			//cout<<DescentDirection.transpose()<<endl;
			//cout<<DescentDirection/norm_current<<endl;
			//cout<<DescentDirection/norm_current*UserReferencedLength_<<endl;
			DescentDirection = DescentDirection/norm_current*UserReferencedLength_;
		}
	}


	// step 4.2 : compute step length
	vector<double> DescentDirectionVector;
	for(int i=0;i<SizeVariables_;i++)
	{
		DescentDirectionVector.push_back(DescentDirection(i,0));
	}

	alpha_stepLength_ = GetAlphaStepLength_LineSearch_0816Method(VariablesPrevious,DescentDirectionVector);

	// step 5 : get current variables
	MatrixXd stepLength_matrix(SizeVariables_,1);
	stepLength_matrix = alpha_stepLength_*DescentDirection;

	stepLengths_.clear();
	for(int i=0;i<SizeVariables_;i++)
	{
		int variableID = i;
		double stepLength_currentID = stepLength_matrix(variableID,0);
		double variable_currentID = VariablesPrevious_[variableID] + stepLength_currentID;
		stepLengths_.push_back(stepLength_currentID);
		VariablesCurrent.push_back(variable_currentID);
	}
	VariablesCurrent_ = VariablesCurrent;


	/*
	// debug
	cout<<"----------------------------------------"<<endl;
	cout<<"----"<<endl;
	cout<<"NewtonsOptimizationManager::Iteration"<<endl;

	vector<double> costFunctionValues_previous;
	vector<double> costFunctionValues_current;
	bool isPreviousGood = costFunction_->CostFunction(VariablesPrevious_,costFunctionValues_previous);
	bool isCurrentGood = costFunction_->CostFunction(VariablesCurrent_,costFunctionValues_current);
	cout<<"Cost Function,  previous "<<costFunctionValues_previous[0]<<"; current "<<costFunctionValues_current[0]<<endl;

	cout<<"Eigen values of Heesian "<<eigenSolver.eigenvalues().transpose()<<endl;
	if(areAllEigenValuesPositive)
	{
		cout<<"The Hessian matrix is positive definite, the Newton's method is going to be uesd"<<endl;
	}
	else
	{
		cout<<"The Hessian matrix is not positive definite, the steepest descent method is going to be uesd"<<endl;
	}

	cout<<"Step Length computing : alpha_stepLength_ "<<alpha_stepLength_<<endl;
	cout<<"derivatives of current variables : "<<endl;
	for(int i=0;i<SizeVariables_;i++)
	{
		int variableID = i;
		cout<<"    variableID "<<variableID<<": previous variable "<<VariablesPrevious_[i]<<", stepLength_matrix "<<stepLength_matrix(variableID,0)<<", current variable "<<VariablesCurrent_[variableID]<<endl;
	}
	cout<<"----"<<endl;
	cout<<"----------------------------------------"<<endl;
	*/
}

//-------------------------
// Public : output data
//-------------------------
void NewtonsOptimizationManager::EndOfIteration()
{

	for(int i=0;i<SizeVariables_;i++)
	{
		int variableID = i;
		*write<<VariablesCurrent_[i]<<" ";
	}
	*write<<endl;
}
