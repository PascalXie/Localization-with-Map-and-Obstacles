#include "PolyResidualBlockFunction.hh"

//-------------------------
// Constructor
//-------------------------
PolyResidualBlockFunction::PolyResidualBlockFunction(string name, vector<double> observations, int SizeObservations, int SizeVariables, int SizeResiduals)
:	UserResidualBlockFunction(name+"_User"),
	name_(name),
	SizeObservations_(SizeObservations),
	SizeVariables_(SizeVariables), SizeResiduals_(SizeResiduals)
{
	observations_.clear();
	observations_ = observations;
}

//-------------------------
// Destructor
//-------------------------
PolyResidualBlockFunction::~PolyResidualBlockFunction()
{}

//-------------------------
// Public
//-------------------------
bool PolyResidualBlockFunction::ResidualFunction(vector<double> variables, vector<double> &residuals)
{
	//
	// step 0
	//
	if(variables.size()!=SizeVariables_)
	{
		cout<<"An error happend in PolyResidualBlockFunction::ResidualFunction"<<endl;
		return false;
	}
	if(observations_.size()!=SizeObservations_)
	{
		cout<<"An error happend in PolyResidualBlockFunction::ResidualFunction"<<endl;
		return false;
	}

	// observations_[0] : distanceSquared 
	// observations_[1] : ax , anchor location x
	// observations_[2] : ay , anchor location y

	// varialbles[0] : xx, node location x
	// varialbles[1] : xy, node location y

	// residual = (ax-xx)*(ax-xx) + (ay-xy)*(ay-xy) - distanceSquared

	int TotalNodeNumber = 10;
	if(variables.size()!=TotalNodeNumber*2)
	{
		cout<<"An error happend in PolyResidualBlockFunction::ResidualFunction : Node Number Wrong"<<endl;
		return false;
	}

	double ax = observations_[SizeObservations_-2];
	double ay = observations_[SizeObservations_-1];

	double residual_0 = 0;
	for(int i=0;i<TotalNodeNumber;i++)
	{
		int NodeID = i;
		double distanceSquared = observations_[NodeID];
		int xID = 0 + NodeID*2;
		int yID = 1 + NodeID*2;

		double xx = variables[xID];
		double xy = variables[yID];

		double residual_node_0 = sqrt((ax-xx)*(ax-xx) + (ay-xy)*(ay-xy)) - sqrt(distanceSquared);
		double residual_node = residual_node_0*residual_node_0;

		residual_0 += residual_node;

		/*
		// debug
		cout<<"Debug class PolyResidualBlockFunction::ResidualFunction"<<endl;
		cout<<"NodeID "<<NodeID<<endl;
		cout<<"xx "<<xx<<", xy "<<xy<<endl;
		cout<<"ax "<<ax<<", ay "<<ay<<endl;
		cout<<"distanceSquared "<<distanceSquared<<endl;
		*/
	}

	residual_0 = residual_0/double(TotalNodeNumber);

	residuals.clear();
	residuals.push_back(residual_0);

	/*
	// debug
	cout<<"Debug class PolyResidualBlockFunction::ResidualFunction"<<endl;
	cout<<"Debug Residual : Observation "<<distanceSquared<<" "<<ax<<" "<<ay<<endl;
	cout<<"Debug Residual : varialbles "<<xx<<" "<<xy<<", residual "<<residual_0<<endl;
	*/

	return true;
}

int PolyResidualBlockFunction::GetObervationsSize()
{
	return SizeObservations_;
}

void PolyResidualBlockFunction::ShowResidualFunction()
{
	// debug
	cout<<"debug Class PolyResidualBlockFunction : "<<name_<<endl;
	cout<<"Observations : "<<endl;
	for(int i=0; i<observations_.size(); i++)
	{
		cout<<"    ID "<<i<<"; Observation "<<observations_[i]<<endl;
	}
}
