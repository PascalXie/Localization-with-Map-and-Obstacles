#ifndef NEWTONSOPTIMIZATIONMANAGER_HH
#define NEWTONSOPTIMIZATIONMANAGER_HH

#include <iostream>
#include <string>
#include <vector>
#include <cmath>

// Eigen
#include <Eigen/Core>
#include <Eigen/Dense>

#include "UserOptimizationManager.hh"

#include "NewtonsCostFunction.hh"

using namespace std;
using namespace Eigen;

class NewtonsOptimizationManager : virtual public UserOptimizationManager
{
    public:
		NewtonsOptimizationManager(string name, int ObservationsSize, int VariablesSize, int ResidualsSize);
		~NewtonsOptimizationManager();

    public:
		virtual void SetUserInitialization(UserCostFunction* costFunction);
		virtual void SetUserInitialization(vector<double> variables);
		virtual void Iteration(vector<double> VariablesPrevious);
    public:
		virtual void EndOfIteration();

    //public:
	//	// line search methods
	//	double GetAlphaStepLength_LineSearch_0816Method(vector<double> Variables, vector<double> DescentDirection);

    private:
		string name_;

		// Jacobian
		MatrixXd JacobianMatrix_;

		// Hessian
		MatrixXd HessianMatrix_;

		// output data
		ofstream *write;


};
#endif
