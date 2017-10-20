void errorsMC(string filename = "test.root")
{
	gROOT->Reset(); 
	string cut = "B0_mbc > 5.27 && B0_deltae > -0.2 && B0_deltae < 0.1"; // BEST
	string mccut= "B0_isSignal &&";
	int nevents = 40000;
	TCanvas * c1 = new TCanvas("c1", "Dalitz",0,0, 500,500);
	TFile * file = TFile::Open(filename.c_str());
	//TTree* K0Signal = (TTree*)file->Get("K0Signal");
	TTree* B0Signal = (TTree*)file->Get("B0Signal");
        TH1F* mcErrHist = new TH1F("mcErrHist",";code",1600,0,1600);
	int total = B0Signal->Project("mcErrHist","B0_mcErrors",(cut).c_str());
	float absCorrect = mcErrHist->GetBinContent(3);
	std::cout << "Abs correct: " << absCorrect << std::endl;
	mcErrHist->Draw();
}	
