#include "ToolSignalPowerGenerator.hh"

//---------------------------
// Constructor
//---------------------------
ToolSignalPowerGenerator::ToolSignalPowerGenerator(string name)
:	name_(name),
	isMapGood_(false),
	binSize_(100),
	C_(1.)
{

	/*
	// debug
	cout<<"Name "<<name_<<endl;
	*/
}

//---------------------------
// Deconstructor
//---------------------------
ToolSignalPowerGenerator::~ToolSignalPowerGenerator()
{}

//---------------------------
// Public
//---------------------------
void ToolSignalPowerGenerator::SetToolMapGenerator(ToolMapGenerator *map)
{
	map_ = map;
	isMapGood_ = true;
}

//---------------------------
// Public : compute
//---------------------------
double ToolSignalPowerGenerator::GetSignalPower(double S_node, vector<double> A_anchor, vector<double> A_node)
{
	// S_node : Signal Power at the node
	// A_anchor : position of the anchor , i.e. Beacon
	// A_anchor : x, y, z; for 2D and 3D
	// A_node : position of the node
	// A_node : x, y, z; for 2D and 3D

	// for 2D : A_anchor[2] = A_node[2], i.e. A_anchor[2] = A_node[2] = 0

	//
	// step 0 : debug
	//
	if(A_anchor.size()!=3)
	{
		cout<<"An error happend in ToolSignalPowerGenerator::GetSignalPower, A_anchor.size must be 3"<<endl;
		return 0;
	}

	if(A_node.size()!=3)
	{
		cout<<"An error happend in ToolSignalPowerGenerator::GetSignalPower, A_node.size must be 3"<<endl;
		return 0;
	}

	//
	// step 1 : compute binWidth
	//
	vector<double> binWidth;
	for(int i=0;i<3;i++)
	{
		double width = (A_anchor[i]-A_node[i])/double(binSize_);
		binWidth.push_back(width);

		/*
		// debug
		cout<<"bin width ID "<<i<<", witdh "<<width<<endl;
		*/
	}


	//
	// step 2 : integral
	//
	// integral = \int -2*constant*delta
	double integral = 0;

	double delta = sqrt(binWidth[0]*binWidth[0]+binWidth[1]*binWidth[1]+binWidth[2]*binWidth[2]);

	for(int i=0;i<binSize_;i++)
	{
		double x = A_node[0] + (double(i)+0.5)*binWidth[0];
		double y = A_node[1] + (double(i)+0.5)*binWidth[1];
		double z = A_node[2] + (double(i)+0.5)*binWidth[2];

		double constant = map_->GetConstant(x,y,z);

		integral = integral + (-2.*constant*delta);

		/*
		// debug
		cout<<"loop ID "<<i<<": x "<<x<<", y "<<y<<", z "<<z<<"; constant "<<constant<<endl;
		*/
	}


	//
	// step 3 : compute the Factor
	//
	// 1/(4.*PI*delta^2)
	E_ = C_/(4.*PI*delta*delta);

	//
	// step 4 : compute power at the anchor
	//
	//double Power_anchor = 
	double S_anchor = S_node * E_ * exp(integral);

	// debug
	cout<<"------------------------------------------------"<<endl;
	cout<<"ToolSignalPowerGenerator::GetSignalPower"<<endl;
	cout<<"delta "<<delta<<endl;
	cout<<"Factor "<<E_<<endl;
	cout<<"integral "<<integral<<endl;
	cout<<"Power Node : "<<S_node<<endl;
	cout<<"Power Anchor : "<<S_anchor<<endl;
	cout<<"------------------------------------------------"<<endl;

	return S_anchor;
}
