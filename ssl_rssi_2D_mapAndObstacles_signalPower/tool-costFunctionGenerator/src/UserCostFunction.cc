#include "UserCostFunction.hh"

//-------------------------
// Constructor
//-------------------------
UserCostFunction::UserCostFunction(string name, int SizeObservations, int SizeVariables, int SizeResiduals)
:	name_(name),
	SizeObservations_(SizeObservations),
	SizeVariables_(SizeVariables),
    SizeResiduals_(SizeResiduals)
{
	//cout<<"UserCostFunction "<<name_<<endl;
	ResidualBlockFunctions_.clear();
}

//-------------------------
// Destructor
//-------------------------
UserCostFunction::~UserCostFunction()
{}

//-------------------------
// Public
//-------------------------
bool UserCostFunction::GradientFunction(vector<double> variables, vector<double> &theDerivatives)
{
	// calculate all derivatives
	vector<double> derivatives;
	derivatives.clear();

	// debug
	if(variables.size()!=SizeVariables_)
	{
		cout<<"An Error happend in Class UserCostFunction::GradientFunction "<<endl;
		return false;
	}

	// step 1 : get derivatives one by one
	for(int i=0;i<SizeVariables_;i++)
	{
		/*
		// debug
		cout<<"Debug varialbeID "<<i<<endl;
		cout<<"Debug SizeVariables_ "<<SizeVariables_<<endl;
		*/

		int varialbeID = i;
		double derivative_current = 888;
		bool isDerivativeGood = GetOneDerivative(varialbeID,variables,derivative_current);
		derivatives.push_back(derivative_current);
	}

	// step 2 : output derivatives
	theDerivatives = derivatives;

	return true;
}

bool UserCostFunction::HessianMatrixFunction(vector<double> variables, vector<double> &HessianMatrix)
{
	// make Hessian Matrix ready : clearing and indexing
	vector<double> theHessianMatrix;
	theHessianMatrix.clear();
	for(int i=0;i<SizeVariables_;i++)
	for(int j=0;j<SizeVariables_;j++)
	{
		theHessianMatrix.push_back(0);

		//// indexing, illustration use, useless here
		//int RowID = i;
		//int ColumnID = j;
		//int ID = RowID*SizeVariables_ + ColumnID;
	}

	// get Hessian Matrix
	for(int i=0;i<SizeVariables_;i++)
	{
		int RowID = i;
		for(int j=0;j<SizeVariables_;j++)
		{
			int ColumnID = j;
			
			double element = 888;
			bool isOneHessianElementGood = GetOneSecondOrderDerivative(RowID,ColumnID,variables,element);
			if(isOneHessianElementGood==false)
			{
				cout<<"An Error happend in UserCostFunction::HessianMatrixFunction"<<endl;
				return false;
			}

			int ID = RowID*SizeVariables_ + ColumnID;
			theHessianMatrix[ID] = element;

			/*
			// debug
			cout<<"elementID "<<ID<<", element "<<element<<endl;
			*/
		}
	}

	HessianMatrix = theHessianMatrix;

	return true;
}
