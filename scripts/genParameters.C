
void genParameters(string filename = "mc.root")
{
	TCanvas * c1 = new TCanvas("c1", "Dalitz",0,0, 1500,1000);
	c1->Divide(3,2);
	TFile * file = TFile::Open(filename.c_str());
	TTree* B0sSIG = (TTree*)file->Get("B0sSIG");

	c1->cd(1);
	TH1F * piPHist = new TH1F("piPHist", ";|p|(#pi^#pm) [GeV]", 50,0,3);
	B0sSIG->Project("piPHist","B0_pi0_P");
	piPHist->SetLineWidth(3);
	piPHist->SetLineColor(kBlue);
	piPHist->Draw("he");
	
	
	c1->cd(2);
	TH1F * picosHist = new TH1F("picosHist", ";cos#theta(#pi^#pm) [GeV]", 50,-1,1);
	B0sSIG->Project("picosHist","B0_pi0__cosTheta");
	picosHist->SetLineWidth(3);
	picosHist->SetMinimum(0);
	picosHist->SetLineColor(kBlue);
	picosHist->Draw("he");
	
	TTree* K0sMy = (TTree*)file->Get("K0sMy");
	c1->cd(4);
	TH1F * kPHist = new TH1F("kPHist", ";|p|(#pi^{#pm}) from K^{0}_{S} [GeV]", 50,0,3);
	K0sMy->Project("kPHist","K_S0_pi0_P","abs(K_S0_MC_MOTHER_ID) == 10313 || abs(K_S0_MC_MOTHER_ID) == 323");
	kPHist->SetLineWidth(3);
	kPHist->SetLineColor(kBlue);
	kPHist->Draw("he");
	
	
	c1->cd(5);
	TH1F * kcosHist = new TH1F("kcosHist", ";cos#theta(#pi^{#pm}) from K^{0}_{S}", 50,-1,1);
	K0sMy->Project("kcosHist","K_S0_pi0__cosTheta","abs(K_S0_MC_MOTHER_ID) == 10313 || abs(K_S0_MC_MOTHER_ID) == 323");
	kcosHist->SetLineWidth(3);
	kcosHist->SetMinimum(0);
	kcosHist->SetLineColor(kBlue);
	kcosHist->Draw("he");
}

