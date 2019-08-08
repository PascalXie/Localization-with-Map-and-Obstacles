#ifndef USERRESIDUALBLOCKFUNCTION_HH
#define USERRESIDUALBLOCKFUNCTION_HH

#include <iostream>
#include <string>
#include <vector>

using namespace std;

class UserResidualBlockFunction
{
	public:
		UserResidualBlockFunction(string name);
		~UserResidualBlockFunction();

	public:
		virtual int GetObervationsSize() = 0;
		virtual bool ResidualFunction(vector<double> variables, vector<double> &residuals) = 0;

    public:
		virtual void ShowResidualFunction() = 0;
	
    private:
		string name_;
};
#endif
