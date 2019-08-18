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

	// Build map and signal power generators
	map_ = new ToolMapGenerator("Map","3D"); 
	map_->SetSigma(10); 
	map_->ImportSamples(); 

	spg_ = new ToolSignalPowerGenerator("spg");
	spg_->SetToolMapGenerator(map_); 
	spg_->SetFactor(1e-1);
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
	// observations_[1] : S_n, node power, by node 1 
	// observations_[2] : S_ao, anchor observed power, by node 2
	// observations_[3] : S_n, node power, by node 2 
	// observations_[4] : ax , anchor location x
	// observations_[5] : ay , anchor location y

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

	// anchor postion
	double ax = observations_[SizeObservations_-2];
	double ay = observations_[SizeObservations_-1];
	double az = 0;

	vector<double> A_Anchor; // position of the anchor
	A_Anchor.push_back(ax);
	A_Anchor.push_back(ay);
	A_Anchor.push_back(az);

	// power
	double residual_0 = 0;
	for(int i=0;i<TotalNodeNumber;i++)
	{
		int NodeID = i;

		int obsID = 0 + NodeID*2;  // signal power observed
		int souID = 1 + NodeID*2; // signal power at the source, i.e. the node

		double S_ao = observations_[obsID]; // the observed signal strength for anchor
		double S_node = observations_[souID]; // signal power at the anchor

		// target node position
		double xx = variables[obsID];
		double xy = variables[souID];

		vector<double> A_Node; // position of the node 
		A_Node.push_back(xx);
		A_Node.push_back(xy);
		A_Node.push_back(0);

		double S_a = spg_->GetSignalPowerStrength_dBm(S_node,A_Anchor,A_Node);

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
