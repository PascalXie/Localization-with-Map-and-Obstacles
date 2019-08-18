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

	/*
	// Build map and signal power generators
	map_ = new ToolMapGenerator("Map","3D"); 
	map_->SetSigma(10); 
	map_->ImportSamples(); 

	spg_ = new ToolSignalPowerGenerator("spg");
	spg_->SetToolMapGenerator(map_); 
	spg_->SetFactor(1e-1);
	*/
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

	// observations_[0] : S_ao, anchor observed power, by node 1
	// observations_[1] : S_ref, referenced power, by node 1 
	// observations_[2] : S_ao, anchor observed power, by node 2
	// observations_[3] : S_ref, referenced power, by node 2 
	// observations_[4] : PathLossExponent, PathLossExponent 
	// observations_[5] : d_0, referenced distance
	// observations_[6] : ax , anchor location x
	// observations_[7] : ay , anchor location y

	// varialbles[0] : xx, node location x, node 1
	// varialbles[1] : xy, node location y, node 1
	// varialbles[2] : xx, node location x, node 2
	// varialbles[3] : xy, node location y, node 2


	// S_a = spg_->GetSignalPower, the computed signal power observed by the anchor

	// residual = sum of (S_a - S_ao)^2 for all nodes
	// residual = (S_a_n1 - S_ao_n1)^2 + (S_a_n2 - S_ao_n2)^2

	int TotalNodeNumber = 1;
	if(variables.size()!=TotalNodeNumber*2)
	{
		cout<<"An error happend in PolyResidualBlockFunction::ResidualFunction : Node Number Wrong"<<endl;
		return false;
	}

	// Shadowing Parameters
	double PathLossExponent = observations_[SizeObservations_-4];
	double d_0 = observations_[SizeObservations_-3];

	// anchor postion
	double ax = observations_[SizeObservations_-2];
	double ay = observations_[SizeObservations_-1];
	double az = 0;

	// power
	double residual_0 = 0;
	for(int i=0;i<TotalNodeNumber;i++)
	{
		int NodeID = i;

		int obsID = 0 + NodeID*2;  // signal power observed
		int souID = 1 + NodeID*2; // referecend signal power

		double S_ao = observations_[obsID]; // the observed signal strength for anchor
		double S_ref = observations_[souID]; // referenced signal power 

		// target node position
		double xx = variables[obsID];
		double xy = variables[souID];
		double xz = 0;

		double d = sqrt((ax-xx)*(ax-xx)+(ay-xy)*(ay-xy)+(az-xz)*(az-xz));
		double S_a = S_ref - 10.*PathLossExponent*log10(d/d_0);

		double residual_node = sqrt((S_ao-S_a)*(S_ao-S_a));
		residual_0 += residual_node;

		/*
		// debug
		cout<<"Debug class PolyResidualBlockFunction::ResidualFunction"<<endl;
		cout<<"NodeID "<<NodeID<<endl;
		cout<<"xx "<<xx<<", xy "<<xy<<endl;
		cout<<"Anchor : "<<A_Anchor[0]<<", "<<A_Anchor[1]<<", "<<A_Anchor[2]<<endl;
		cout<<"Node : "<<A_Node[0]<<", "<<A_Node[1]<<", "<<A_Node[2]<<"; "<<S_node<<endl;
		cout<<"Power Observed : "<<S_ao<<", computed "<<S_a<<endl;
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


void PolyResidualBlockFunction::SetToolSignalPowerGenerator(ToolSignalPowerGenerator *spg)
{
	spg_ = spg;
}
