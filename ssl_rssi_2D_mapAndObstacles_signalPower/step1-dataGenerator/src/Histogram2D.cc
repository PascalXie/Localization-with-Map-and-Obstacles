#include "Histogram2D.hh"

//----------------------------
// Constructor
//----------------------------
Histogram2D::Histogram2D(string name, int binSizex, double minx, double maxx, int binSizey, double miny, double maxy)
	:name_(name),
	binSizex_(binSizex),
	minx_(minx),
	maxx_(maxx),
	binSizey_(binSizey),
	miny_(miny),
	maxy_(maxy)
{
	binWidthx_ = (maxx_-minx_)/double(binSizex_);
	binWidthy_ = (maxy_-miny_)/double(binSizey_);

	for(int i=0;i<binSizex_;i++)
	{
		int xID = i;
		string hisyName = "hisx_" + to_string(xID); 
		Histogram hisy(hisyName,binSizey_,miny_,maxy_);
		histogramYs_.push_back(hisy);
	}

	// debug
	cout<<"-------------------------------"<<endl;
	cout<<"----"<<endl;
	cout<<"Histogram "<<name_<<endl;

	cout<<"binSizex_ "<<binSizex_<<", ";
	cout<<"minx_ "	 <<minx_<<", ";
	cout<<"maxx_ "	 <<maxx_<<", ";
	cout<<"binWidthx_ "	 <<binWidthx_<<endl;

	cout<<"binSizey_ "<<binSizey_<<", ";
	cout<<"miny_ "	 <<miny_<<", ";
	cout<<"maxy_ "	 <<maxy_<<", ";
	cout<<"binWidthy_ "	 <<binWidthy_<<endl;
	cout<<"----"<<endl;
	cout<<"-------------------------------"<<endl;

}

//----------------------------
// Deconstructor
//----------------------------
Histogram2D::~Histogram2D()
{}

//----------------------------
// Public
//----------------------------
void Histogram2D::Fill(double x, double y, double weight)
{
	// step 1 : locate X, which means locate the histogram in histogramYs_

	// step 1.1 : find the bin corresponding to the sample value
	int binxID = GetBinIDX(x);

	// step 1.2 : fill the weighth into current bin
	histogramYs_[binxID].Fill(y,weight);
}

//----------------------------
// Private : Tools
//----------------------------
int Histogram2D::GetBinIDX(double x)
{
	int binxID = -1;

	// step 1.1 : find the bin corresponding to the sample value
	for(int i=0;i<binSizex_;i++)
	{
		int binxIDCurrent = i;
		double minxCurrentBin = 0;
		double maxxCurrentBin = 0;
		int isBinBorderGood = GetBinX_Border(binxIDCurrent,minxCurrentBin,maxxCurrentBin);

		if(x>=minxCurrentBin&&x<maxxCurrentBin)
		{
			binxID = binxIDCurrent;
			break;
		}
	}

	return binxID;
}

int Histogram2D::GetBinIDY(double y)
{
	int binyID = histogramYs_[0].GetBinID(y);
	return binyID;
}

bool Histogram2D::GetBinCentor2D(int binIDx, int binIDy, double &x, double &y)
{
	x = minx_ + binWidthx_*(double(binIDx)+0.5);
	y = miny_ + binWidthy_*(double(binIDy)+0.5);

	return true;
}

//----------------------------
// Public
//----------------------------
void Histogram2D::Show()
{
	for(int i=0;i<binSizex_;i++)
	{
		int binxID = i;
		histogramYs_[binxID].Show();
	}
}

//----------------------------
// Private 
//----------------------------
double Histogram2D::GetBinContent(int binIDx, int binIDy)
{
	if(binIDx>=binSizex_||binIDy>=binSizey_)
	{
		cout<<"An error happend in Histogram2D::GetBinContent"<<endl;
		return -1;
	}

	double binContent = histogramYs_[binIDx].GetBinContent(binIDy);
	return binContent;
}

//----------------------------
// Private 
//----------------------------
int Histogram2D::GetBinX_Border(int currentBin, double &minCurrentBin, double &maxCurrentBin)
{
	if(currentBin<0) return 0;

	minCurrentBin = minx_ +  binWidthx_*(double(currentBin));
	maxCurrentBin = minx_ +  binWidthx_*(double(currentBin) + 1.);

	return 1;
}

int Histogram2D::GetBinXY_Border(int Binx, int Biny, double &minBinx, double &maxBinx, double &minBiny, double &maxBiny)
{
	// X 
	int isXGood = GetBinX_Border(Binx,minBinx,maxBinx);

	// Y
	int isYGood = histogramYs_[0].GetBin_Border(Biny,minBiny,maxBiny);

	return 1;
}
