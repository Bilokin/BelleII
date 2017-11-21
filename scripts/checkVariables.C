
void checkVariables(string filename = "test.root", bool signal = true)
{
	string trueB = "B0_isSignal == 1";
	string scfB = "B0_isSignal == 0 && B0_isContinuumEvent == 0";
	string continuum = "B0_isSignal == 0 && B0_isContinuumEvent == 1";
	string cut = (signal)? trueB : continuum;
	cut += " && iCand == 0 ";
	//cut = "iCand == 0 ";	
	cut += " && abs(B0_K_10_K_S0_SigM) < 3 ";
	cut += " && B0_K_10_K_S0_Rho > 0.15";
	cut += " && B0_K_10_K_S0_VtxPvalue > 0.01";
	TCanvas * c1 = new TCanvas("c1", "Dalitz",0,0, 1000,1000);
	c1->Divide(2,2);
	TFile * file = TFile::Open(filename.c_str());
	TTree* B0Signal = (TTree*)file->Get("B0Signal");
	c1->cd(1);
	int nbins = 100;
	TH2F * dEMbcHist = new TH2F("dEMbcHist", "; #Delta E [GeV];M_{bc} [GeV]", nbins,-0.2,0.2, nbins,5.2,5.3);
	B0Signal->Project("dEMbcHist", "B0_mbc : B0_deltae", ( cut ).c_str());
	dEMbcHist->Draw("colz");
	c1->cd(2);
	TH1F * dEHist = new TH1F("dEHist", ";#Delta E [GeV]", nbins,-0.2,0.2);
	B0Signal->Project("dEHist", "B0_deltae", ( cut ).c_str());
	makePretty(dEHist);
	dEHist->Draw("he");
	c1->cd(3);
	TH1F * MbcHist = new TH1F("MbcHist", ";M_{bc} [GeV]", nbins,5.2,5.3);
	B0Signal->Project("MbcHist", "B0_mbc", ( cut ).c_str());
	makePretty(MbcHist);
	MbcHist->Draw("he");
	THStack * stack = new THStack("stack","stack");
	
}
