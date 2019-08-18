#include "ToolMapGenerator.hh"

//---------------------------
// Constructor
//---------------------------
ToolMapGenerator::ToolMapGenerator(string name, string MapType)
:	name_(name),
	MapType_("Empty"),
	IsMapTypeCorrect_(true),
	PI_(3.1415926),
	sigma_(10)
{
	if(MapType=="2D")		MapType_=MapType;
	else if(MapType=="3D")  MapType_=MapType;
	else
	{
		IsMapTypeCorrect_ = false;
	}

	/*
	// debug
	cout<<"Name "<<name_<<endl;
	cout<<"MapType_ "<<MapType_<<", IsMapTypeCorrect_ "<<IsMapTypeCorrect_<<endl;
	*/
}

//---------------------------
// Deconstructor
//---------------------------
ToolMapGenerator::~ToolMapGenerator()
{}

//---------------------------
// Public
//---------------------------
void ToolMapGenerator::SetSample(double constant, double x, double y, double z)
{
	if(MapType_=="2D") z=0;

	constants_.push_back(constant);
	xs_.push_back(x);
	ys_.push_back(y);
	zs_.push_back(z);

	/*
	// debug
	cout<<"x "<<x<<", y "<<y<<", z "<<z<<endl;
	*/
}

void ToolMapGenerator::SetSigma(double sigma)
{
	sigma_ = sigma;
}

double ToolMapGenerator::GetConstant(double x, double y, double z)
{
	if(MapType_=="2D") z=0;

	double weightsTotal = 0;
	double weightedConstantTotal = 0;
	for(int i=0;i<xs_.size();i++)
	{
		double ax = xs_[i];
		double ay = ys_[i];
		double az = zs_[i];
		double distance2 = (x-ax)*(x-ax) + (y-ay)*(y-ay) + (z-az)*(z-az);
		double distance = sqrt(distance2);

		double weight = GaussianFunction(0, sigma_, distance);
		if(weight==0) continue;

		double weightedConstant = constants_[i]*weight;

		weightedConstantTotal += weightedConstant;
		weightsTotal += weight;
	}

	double constant = 0;
	if(weightsTotal==0)
	{
		//cout<<"weightsTotal==0"<<endl;
		constant = 0;
	}
	else
	{
		constant = 1./weightsTotal*weightedConstantTotal;
	}


	/*
	// debug
	cout<<"ToolMapGenerator::GetConstant"<<endl;
	cout<<"weightsTotal "<<weightsTotal<<", ";
	cout<<"weightedConstantTotal "<<weightedConstantTotal<<", ";
	cout<<"constant "<<constant<<endl;
	*/

	return constant;
}

//---------------------------
// Public
//---------------------------
void ToolMapGenerator::OutputSamples()
{
	// debug
	if(IsMapTypeCorrect_==false) 
	{
		cout<<"An error happend in ToolMapGenerator::OutputSamples"<<endl;
		return ;
	}

	// output samples
	ofstream write("Data_MapNodeSamples.txt");

	for(int i=0;i<xs_.size();i++)
	{
		write<<xs_[i]<<" "<<ys_[i]<<" "<<zs_[i]<<" "<<constants_[i]<<endl;
	}

	write.close();
}

bool ToolMapGenerator::ImportSamples(string filename)
{
	ifstream read(filename);

	// debug
	if(read.fail())
	{
		cout<<"An error happend in ToolMapGenerator::ImportSamples"<<endl;
		cout<<"Data file \" "<<filename<<" \" did not exist"<<endl;
		return false;
	}

	// clear 
	xs_.clear();
	ys_.clear();
	zs_.clear();
	constants_.clear();

	// read
	while(!read.eof())
	{
		double x, y, z, c;
		read>>x>>y>>z>>c;

		if(read.eof()) break;

		xs_.push_back(x);
		ys_.push_back(y);
		zs_.push_back(z);
		constants_.push_back(c);

		/*
		// debug
		cout<<x<<" "<<y<<" "<<z<<" "<<c<<endl;
		*/
	}

	read.close();

	/*
	// debug
	cout<<"Debug ToolMapGenerator::ImportSamples"<<endl;
	for(int i=0;i<xs_.size();i++)
	{
		cout<<"Sample ID "<<i<<": "<<xs_[i]<<", "<<ys_[i]<<", "<<zs_[i]<<"; "<<constants_[i]<<endl;
	}
	*/

	return true;
}

//---------------------------
// Public
//---------------------------
void ToolMapGenerator::GetMap(int binx, double minx, double maxx,int biny, double miny, double maxy,int binz, double minz, double maxz)
{
	if(MapType_=="2D") {
		binz = 1;
		minz = 0;
		maxz = 0;
	}

	ofstream write("log_mapOfConstants.txt");
	write<<binx<<" "<<minx<<" "<<maxx<<" "<<biny<<" "<<miny<<" "<<maxy<<" "<<binz<<" "<<minz<<" "<<maxz<<endl;

	double binWidthx = (maxx-minx)/double(binx);
	double binWidthy = (maxy-miny)/double(biny);
	double binWidthz = (maxz-minz)/double(binz);

	for(int k=0;k<binz;k++)
	for(int j=0;j<biny;j++)
	for(int i=0;i<binx;i++)
	{
		double x = minx + (double(i)+0.5)*binWidthx;
		double y = miny + (double(j)+0.5)*binWidthy;
		double z = minz + (double(k)+0.5)*binWidthz;

		double constant = GetConstant(x,y,z);

		write<<x<<" "<<y<<" "<<z<<" "<<constant<<endl;
	}

	write.close();
}

//---------------------------
// Private
//---------------------------
double ToolMapGenerator::GaussianFunction(double mu, double sigma, double value)
{
	// Gaussian distribution function
	double part1 = 1./(sqrt(2.*PI_)*sigma);
	double part2_1 = -1.*(value-mu)*(value-mu)/(2.*sigma*sigma);
	double part2 = exp(part2_1);

	double probabilityDensity = part1*part2;
	if(probabilityDensity<1e-3) probabilityDensity=0;

	return probabilityDensity;
}
