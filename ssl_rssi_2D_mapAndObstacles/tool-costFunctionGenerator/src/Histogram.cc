#include "Histogram.hh"

Histogram::Histogram(string name, int binSize, double min, double max)
:	name_(name),
	binSize_(binSize),
	min_(min),
	max_(max)
{
	binWidth_ = (max_-min_)/double(binSize_);

	cout<<"Histogram "<<name_<<endl;
	cout<<"binSize_ "<<binSize_<<endl;
	cout<<"min_ "	 <<min_<<endl;
	cout<<"max_ "	 <<max_<<endl;
	cout<<"binWidth_ "	 <<binWidth_<<endl;


	for(int i=0;i<binSize_;i++)
	{
		counts_.push_back(0);
	}
}

Histogram::~Histogram()
{}


//
// public
//
void Histogram::Fill(double value, double weight)
{
	int binID = -1;

	// step 1 : find the bin corresponding to the sample value
	for(int i=0;i<binSize_;i++)
	{
		int binIDCurrent = i;
		double minCurrentBin = 0;
		double maxCurrentBin = 0;
		int isBinBorderGood = GetBin_Border(binIDCurrent,minCurrentBin,maxCurrentBin);

		if(value>=minCurrentBin&&value<maxCurrentBin)
		{
			binID = binIDCurrent;
			break;
		}
	}

	// step 2 : fill the weighth into current bin
	counts_[binID] += weight;
}

void Histogram::Show()
{
	for(int i=0;i<binSize_;i++)
	{
		int binID = i;
		cout<<"binID "<<binID<<", count "<<counts_[binID]<<endl;
	}

}

//
// private
//
int Histogram::GetBin_Border(int currentBin, double &minCurrentBin, double &maxCurrentBin)
{
	if(currentBin<0) return 0;

	minCurrentBin = min_ +  binWidth_*(double(currentBin));
	maxCurrentBin = min_ +  binWidth_*(double(currentBin) + 1.);

	return 1;
}
