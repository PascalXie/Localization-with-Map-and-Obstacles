#include "ToolNodesGenerator.hh"

//--------------------------------
// Constructor
//--------------------------------
ToolNodesGenerator::ToolNodesGenerator(string name, int NumberAnchors, int NumberNodes, string MapType)
:	name_(name),
	NumberAnchors_(NumberAnchors),
	NumberNodes_(NumberNodes),
	MapType_("Empty"),
	IsMapTypeGood_(true),
	engine_(time(nullptr))
{
	// initialize boundary
	minX_ = 0.; 
    maxX_ = 0.;

    minY_ = 0.;
    maxY_ = 0.;

    minZ_ = 0.;
    maxZ_ = 0.;


	for(int i=0;i<NumberAnchors_;i++)
	{
		As_x_.push_back(0);
		As_y_.push_back(0);
		As_z_.push_back(0);
	}

	for(int i=0;i<NumberNodes_;i++)
	{
		Ns_x_.push_back(0);
		Ns_y_.push_back(0);
		Ns_z_.push_back(0);
	}

	// get map type : 
	// 2D for 2D space
	// 3D for 3D space
	if(MapType=="2D") MapType_ = MapType;
	else if(MapType=="3D") MapType_ = MapType;
	else {
		IsMapTypeGood_ = false;
		cout<<"An error happend in Class ToolNodesGenerator::ToolNodesGenerator"<<endl;
	}

	cout<<"-------------------------------------------------------"<<endl;
	cout<<"----"<<endl;
	cout<<"Class ToolNodesGenerator::ToolNodesGenerator "<<endl;
	cout<<"Name "<<name_<<endl;
	cout<<"MapType_ "<<MapType_<<", IsMapTypeGood_ "<<IsMapTypeGood_<<endl;
	cout<<"----"<<endl;
	cout<<"-------------------------------------------------------"<<endl;
}

//--------------------------------
// Deconstructor
//--------------------------------
ToolNodesGenerator::~ToolNodesGenerator()
{}


//--------------------------------
// Public
//--------------------------------
void ToolNodesGenerator::SetBoundaryX(double min, double max)
{
	minX_ = min;
	maxX_ = max;
}

void ToolNodesGenerator::SetBoundaryY(double min, double max)
{
	minY_ = min;
	maxY_ = max;
}

void ToolNodesGenerator::SetBoundaryZ(double min, double max)
{
	minZ_ = min;
	maxZ_ = max;
}

void ToolNodesGenerator::Initialize()
{
	
	bool isXGood = GenerateRandom("X");
	bool isYGood = GenerateRandom("Y");
	bool isZGood = GenerateRandom("Z");

	/*
	// observations
	// write
	ofstream write1("observations_groundTruth"+name_+".txt");
	ofstream write2("observations"+name_+".txt");
	write1<<"NumberAnchors "<<NumberAnchors_<<" NumberNodes "<<NumberNodes_<<endl;
	write2<<"NumberAnchors "<<NumberAnchors_<<" NumberNodes "<<NumberNodes_<<endl;
	for(int i=0;i<NumberAnchors_;i++)
	for(int j=0;j<NumberNodes_;j++)
	{
		int AnchorID = i;
		int NodeID = j;

		double distanceSquared = 0;
		double ax = As_x_[AnchorID];
		double ay = As_y_[AnchorID];
		double az = As_z_[AnchorID];

		double xx = Ns_x_[NodeID];
		double xy = Ns_y_[NodeID];
		double xz = Ns_z_[NodeID];

		//cout<<"Distance : Anchor ID "<<AnchorID<<", loc "<<ax<<", "<<ay<<"; Node ID "<<NodeID<<", loc "<<xx<<", "<<xy<<endl;
		distanceSquared = (ax-xx)*(ax-xx) + (ay-xy)*(ay-xy) + (az-xz)*(az-xz);
		//cout<<"distanceSquared "<<distanceSquared<<endl;

		double distance = sqrt(distanceSquared);
		normal_distribution<double> distancesGenerator(distance, distance/2.);
		double distance_randomized = distancesGenerator(engine_);
		if(distance_randomized<0) distance_randomized=0;
		double distanceSquared_randomized = distance_randomized*distance_randomized;

		if(MapType_=="2D")
		{
			write1<<AnchorID<<" "<<ax<<" "<<ay<<" "<<NodeID<<" "<<xx<<" "<<xy<<" "<<distanceSquared<<endl;
			write2<<AnchorID<<" "<<ax<<" "<<ay<<" "<<NodeID<<" "<<xx<<" "<<xy<<" "<<distanceSquared_randomized<<endl;
		}
		else if(MapType_=="3D")
		{
			write1<<AnchorID<<" "<<ax<<" "<<ay<<" "<<az<<" "<<NodeID<<" "<<xx<<" "<<xy<<" "<<xz<<" "<<distanceSquared<<endl;
			write2<<AnchorID<<" "<<ax<<" "<<ay<<" "<<az<<" "<<NodeID<<" "<<xx<<" "<<xy<<" "<<xz<<" "<<distanceSquared_randomized<<endl;
		}
	}

	write1.close();
	*/

	

	/*
	// debug
	cout<<"-------------------------------------------------------"<<endl;
	cout<<"----"<<endl;
	cout<<"ToolNodesGenerator::Initialize "<<endl;
	cout<<"MapType_ "<<MapType_<<", IsMapTypeGood_ "<<IsMapTypeGood_<<endl;
	cout<<"X : "<<minX_<<", "<<maxX_<<endl;
	cout<<"Y : "<<minY_<<", "<<maxY_<<endl;
	if(MapType_=="3D")
		cout<<"Z : "<<minZ_<<", "<<maxZ_<<endl;
	cout<<"----"<<endl;
	for(int i=0;i<NumberAnchors_;i++)
	{
		cout<<"Anchors : ID "<<i<<", X "<<As_x_[i]<<", Y "<<As_y_[i]<<", Z "<<As_z_[i]<<endl;
	}
	cout<<"----"<<endl;
	for(int i=0;i<NumberNodes_;i++)
	{
		cout<<"Nodes   : ID "<<i<<", X "<<Ns_x_[i]<<", Y "<<Ns_y_[i]<<", Z "<<Ns_z_[i]<<endl;
	}
	cout<<"----"<<endl;
	cout<<"-------------------------------------------------------"<<endl;
	*/

}

//--------------------------------
// Private
//--------------------------------
bool ToolNodesGenerator::GenerateRandom(string AxisName)
{
	double min;
	double max;

	vector<double> As, Ns;

	if(AxisName=="X")
	{
		min = minX_;
		max = maxX_;
	}
	else if(AxisName=="Y")
	{
		min = minY_;
		max = maxY_;
	}
	else if(AxisName=="Z")
	{
		min = minZ_;
		max = maxZ_;
	}
	else
	{
		cout<<"An error happend in ToolNodesGenerator::GenerateRandom"<<endl;
		return false;
	}

	//
	// compute random number
	//
	// anchors
	uniform_real_distribution<double> anchor(min, max);
	for(int i=0;i<NumberAnchors_;i++)
	{
		double a = anchor(engine_);
		As.push_back(a);
	}

	// nodes
	uniform_real_distribution<double> node(min, max);
	for(int i=0;i<NumberNodes_;i++)
	{
		double n = node(engine_);
		Ns.push_back(n);
	}

	// 
	if(AxisName=="X")
	{
		As_x_ = As;
		Ns_x_ = Ns;
	}
	else if(AxisName=="Y")
	{
		As_y_ = As;
		Ns_y_ = Ns;
	}
	else if(AxisName=="Z")
	{
		if(MapType_=="2D") return true;

		As_z_ = As;
		Ns_z_ = Ns;
	}

	return true;
}

//--------------------------------
// Public
//--------------------------------
void ToolNodesGenerator::GetAnchors(vector<double> &As_x, vector<double> &As_y, vector<double> &As_z)
{
	As_x = As_x_;
	As_y = As_y_;
	As_z = As_z_;
}

void ToolNodesGenerator::GetNodes(vector<double> &Ns_x, vector<double> &Ns_y, vector<double> &Ns_z)
{
	Ns_x = Ns_x_;
	Ns_y = Ns_y_;
	Ns_z = Ns_z_;
}
