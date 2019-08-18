#include "ConfidenceRegionManager.hh"

//----------------------------
// Constructor
//----------------------------
ConfidenceRegionManager::ConfidenceRegionManager(string name)
:	name_(name),
	sigma_(10)
{

	cout<<"ConfidenceRegionManager::ConfidenceRegionManager "<<endl;
	cout<<"name_ "<<name_<<endl;
}

//----------------------------
// Deconstructor
//----------------------------
ConfidenceRegionManager::~ConfidenceRegionManager()
{}

//----------------------------
// Public
//----------------------------
void ConfidenceRegionManager::Initialize2DRegion(int binSizex, double minx, double maxx, int binSizey, double miny, double maxy)
{
	//
	binSizex_ = binSizex;
	minx_ = minx;
	maxx_ = maxx;

	binSizey_ = binSizey;
	miny_ = miny;
	maxy_ = maxy;

	binWidthx_ = (maxx_-minx_)/double(binSizex_);
	binWidthy_ = (maxy_-miny_)/double(binSizey_);

	//
	string name = name_ + "_Histogram";
	region2D_ = new Histogram2D(name, binSizex, minx, maxx, binSizey, miny, maxy);

	// get all bins
	for(int i=0;i<binSizex_;i++)
	for(int j=0;j<binSizey_;j++)
	{
		int xID = i;
		int yID = j;
		int ID = xID*binSizey_ + yID;
		double AnchorSize = double(Anxs_.size());
		double Pr = ComputeProbability(xID, yID)/AnchorSize;

		double x = -200;
		double y = -200;
		region2D_->GetBinCentor2D(xID, yID, x, y);

		region2D_->Fill(x,y,Pr);

		/*
		// debug
		cout<<"ID "<<xID<<", "<<yID<<"; x,y "<<x<<", "<<y<<"; Pr "<<Pr<<endl;
		*/
	}

	/*
	// debug
	region2D_->Show();
	*/
}

void ConfidenceRegionManager::OutputHistogram(string filename)
{
	ofstream write(filename);
	write<<"binSizex_ "<<binSizex_<<" binSizey_ "<<binSizey_<<endl;

	// get all bins
	for(int i=0;i<binSizex_;i++)
	for(int j=0;j<binSizey_;j++)
	{
		int xID = i;
		int yID = j;
		double content = region2D_->GetBinContent(xID, yID);

		double x, y;
		bool isCentorGood = region2D_->GetBinCentor2D(xID,yID, x, y);

		write<<xID<<" "<<yID<<" "<<x<<" "<<y<<" "<<content<<endl;
	}

	write.close();
}

double ConfidenceRegionManager::ComputeProbability(int binIDx, int binIDy)
{
	// step 1 : get borders
	double minBinx = 0;
	double maxBinx = 0;
	double minBiny = 0;
	double maxBiny = 0;
	int isXYGood = region2D_->GetBinXY_Border(binIDx,binIDy,minBinx,maxBinx,minBiny,maxBiny);

	//// debug
	//cout<<"binIDx "<<binIDx<<", "<<minBinx<<", "<<maxBinx<<endl;
	//cout<<"binIDy "<<binIDy<<", "<<minBiny<<", "<<maxBiny<<endl;

	// step 2 : compute probabilities
	double Probability = 0; // Probability for current bin

	int binbinSizes[2];
	binbinSizes[0] = 10; // for X axis
	binbinSizes[1] = 10; // for Y axis

	double binbinWidths[2];
	binbinWidths[0] = (maxBinx-minBinx)/double(binbinSizes[0]); // for X axis
	binbinWidths[1] = (maxBiny-minBiny)/double(binbinSizes[1]); // for Y axis

	for(int i=0;i<binbinSizes[0];i++)
	for(int j=0;j<binbinSizes[1];j++)
	{
		int xID = i;
		int yID = j;
		double x = minBinx + binbinWidths[0]*(double(xID)+0.5);
		double y = minBiny + binbinWidths[1]*(double(yID)+0.5);

		for(int k=0;k<Anxs_.size();k++)
		{
			int AnchorID = k;
			double anx = Anxs_[AnchorID];
			double any = Anys_[AnchorID];
			double dis_real = distances_[AnchorID];

			double dis = sqrt((x-anx)*(x-anx)+(y-any)*(y-any));
			double pr = GaussianFunction(dis, dis_real, sigma_);

			Probability = Probability + pr * binbinWidths[0] * binbinWidths[1];

			/*
			// debug
			cout<<"Pr "<<pr * binbinWidths[0] * binbinWidths[1]<<endl;
			*/
		}

		/*
		// debug
		cout<<"ID "<<xID<<" "<<yID<<"; x,y "<<x<<" "<<y<<endl;
		*/
	}

	return Probability;
}

void ConfidenceRegionManager::SetAnchorAndDistance(double x, double y, double distance)
{
	Anxs_.push_back(x);
	Anys_.push_back(y);
	distances_.push_back(distance);

	/*
	// debug
	cout<<"x "<<x<<", y "<<y<<", dis "<<distance<<endl;
	*/
}


//----------------------------
// Private
//----------------------------
double ConfidenceRegionManager::GaussianFunction(double dis, double mu, double sigma)
{
	// Gaussian Distribution : probability density 
	double PI = 3.1415926;
	double part1 = 1./(sqrt(2.*PI)*sigma);
	double part2_expo = -1.*((dis-mu)*(dis-mu)/(2.*sigma*sigma));
	double prDens = part1*exp(part2_expo);

	return prDens;
}
