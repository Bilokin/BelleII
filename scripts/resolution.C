void resolution(string filename = "test.root")
{
	string cut = getCuts();
	string trueB = "&& B0_isSignal == 1";
	TFile * file = TFile::Open(filename.c_str());
	TTree* B0Signal = (TTree*)file->Get("B0Signal");
	TCanvas * c1 = new TCanvas("c1", "Dalitz",0,0, 1000,500);
	c1->Divide(2,1);
	TH1F * deltaTResHist = new TH1F("deltaTResHist", ";#Delta t [ps]", 50,-10,10.);
	TH1F * deltaTResAllHist = new TH1F("deltaTResAllHist", ";Residual #Delta t ", 50,-10,10.);
	
	B0Signal->Project("deltaTResHist", "(B0_DeltaT-B0_TruthDeltaT)", (cut + trueB).c_str());
	B0Signal->Project("deltaTResAllHist", "((B0_DeltaT-B0_TruthDeltaT))", (cut).c_str());
	makePretty(deltaTResHist);
	makePretty(deltaTResAllHist, kGray+1);
	//deltaTResAllHist->Draw();
	//deltaTResHist->Draw("same");
	int nbins = 100;
	TH2F * deltaTRes2Hist = new TH2F("deltaTRes2Hist", "Residual #Delta t ;#Delta t error", nbins,0.3,2.5, 50,-5,5);
	B0Signal->Project("deltaTRes2Hist", "(B0_DeltaT-B0_TruthDeltaT):B0_DeltaTErr", (cut + trueB).c_str());
	c1->cd(1);
	//gPad->SetLogz();
	deltaTRes2Hist->Draw("colz");
	c1->cd(2);
	TProfile* profile = deltaTRes2Hist->ProfileX("Profile",1,-1,"s");
	profile->Draw();
	TH1F * rmsHist = new TH1F("rmsHist", ";axis []", nbins,0.3,2.5);
	for (unsigned int i = 0; i < nbins; i++) 
	{
		rmsHist->SetBinContent(i+1,profile->GetBinError(i+1));
	}
	makePretty(rmsHist);
	rmsHist->Draw("h");
}
