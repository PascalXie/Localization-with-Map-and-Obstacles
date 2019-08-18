#ifndef STEEPESTOPTIMIZATIONMANAGER_HH
#define STEEPESTOPTIMIZATIONMANAGER_HH

#include <iostream>
#include <string>
#include <vector>
#include <cmath>

#include "UserOptimizationManager.hh"

#include "SteepestCostFunction.hh"

using namespace std;

class SteepestOptimizationManager : virtual public UserOptimizationManager
{
    public:
		SteepestOptimizationManager(string name, int ObservationsSize, int VariablesSize, int ResidualsSize);
		~SteepestOptimizationManager();

    public:
		virtual void SetUserInitialization(UserCostFunction* costFunction);
		virtual void SetUserInitialization(vector<double> variables);
		virtual void Iteration(vector<double> VariablesPrevious);
		virtual void EndOfIteration();

    private:
		string name_;
};
#endif
