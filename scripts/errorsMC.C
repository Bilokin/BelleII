#include <sys/ioctl.h>
#include <stdio.h>
#include <unistd.h>
void drawHist(int entries, char symbol = 'O')
{
	std::cout << '\t';
	for (unsigned int i = 0; i < entries; i++) 
	{
		std::cout << symbol;
	}
	std::cout << std::endl;
}
void errorsMC(string filename = "test.root")
{
	struct winsize size;
	ioctl(STDOUT_FILENO,TIOCGWINSZ,&size);
	gROOT->Reset(); 
	string cut = getCuts(); // BEST
	string mccut= "B0_isSignal &&";
	int nevents = 40000;
	TCanvas * c1 = new TCanvas("c1", "Dalitz",0,0, 500,500);
	TFile * file = TFile::Open(filename.c_str());
	//TTree* K0Signal = (TTree*)file->Get("K0Signal");
	TTree* B0Signal = (TTree*)file->Get("B0Signal");
	int nbins = 1700;
        TH1F* mcErrHist = new TH1F("mcErrHist",";code", nbins,0, nbins);
	int total = B0Signal->Project("mcErrHist","B0_mcErrors",(cut).c_str());
	float absCorrect = mcErrHist->GetBinContent(3);
	std::cout << "Abs correct: " << absCorrect << std::endl;
	mcErrHist->Draw();
	for (unsigned int i = 0; i < nbins; i++) 
	{
		int nentries = mcErrHist->GetBinContent(i+1);
		if (nentries) 
		{
			std::cout << "Error\t" << i << ":\t" << nentries;
			drawHist((float)nentries/total*(size.ws_col-27));
		}
	}
}
