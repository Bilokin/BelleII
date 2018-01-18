TH1F * drawHists(TTree* B0Signal, string name, string cut, string title = "Err", float r1 = 0, float r2 = 1, string mccut =  "(B0_isSignal) &&")
{
	string b = "B0_";
	int nbins = 50;	
	string observable = b+name;
	TH1F * xPullHist = new TH1F((name+"Hist").c_str(), (";"+title).c_str(), nbins,r1, r2);
	TH1F * xPullAllHist = new TH1F((name+"AllHist").c_str(), (";"+title).c_str(), nbins, r1, r2);
	
	B0Signal->Project((name+"Hist").c_str(), observable.c_str(),(mccut+cut).c_str());
	B0Signal->Project((name+"AllHist").c_str(), observable.c_str(),(cut).c_str(), "same");
	
	makePretty(xPullHist);
	makePretty(xPullAllHist, kGray+1);
	
	xPullAllHist->Draw("he");
	xPullHist->Draw("hesame");
	return xPullHist;
}

void B0reco(string filename = "test.root")
{
	TCanvas * c1 = new TCanvas("c1", "Dalitz",0,0, 1980,1000);
	c1->Divide(4,2);
	c1->cd(1);
	TFile * file = TFile::Open(filename.c_str());
	TTree* B0Signal = (TTree*)file->Get("B0Signal");
	string cut = getCuts();
	//string mccut= "(B0_isSignal || B0_mcErrors == 258) &&";
	string mccut = "(abs(B0_K_10_mcPDG) == 30343 && abs(B0_gamma_MC_MOTHER_ID) == 511) && ";
	//string mccut= "(B0_isSignal) &&";
	string mccutrho = " abs(B0_K_10_pi0_MC_MOTHER_ID) ==113 &&";
	
	int allB = B0Signal->Draw("B0_mcPDG", (cut).c_str());
	float trueB = B0Signal->Draw("B0_mcPDG",(mccut+cut).c_str());
	int totalEntries = 1000;
	
	TH1F * cosHist = new TH1F("cosHist", ";cos#theta []", 50,0.8,1);
	TH1F * cosAllHist = new TH1F("cosAllHist", ";cos#theta []", 50,0.8,1);
	
	cout << "All B0: " << allB << "; true B0: " << trueB << "\n"; 
	cout << "Purity B0: " << trueB/allB*100 << "%\n";
	drawHists(B0Signal, "deltae", cut, "#DeltaE [GeV]",-0.2,0.2, mccut);
	c1->cd(2);
	drawHists(B0Signal, "M", cut, "M(B_{0}) [GeV]",5,5.5,mccut);
	c1->cd(3);
	drawHists(B0Signal, "mbc", cut, "M_{bc} [GeV]",5.2,5.3,mccut);
	c1->cd(4);
	drawHists(B0Signal, "cosTheta", cut, "cos#Theta",0.8,1,mccut);
	
	c1->cd(5);
	TH1F * rhoHist = drawHists(B0Signal, "K_10_M", cut, "M(K_{1}^{0}) [GeV]",0.8,2, mccut);
	TF1 * myBW1 = new TF1("myBW1","TMath::BreitWigner(x,[0],[1])*[2]*TMath::Gaus(x,[3],[4])",1,2.);
	myBW1->SetParLimits(0,1.2,1.35);
	myBW1->SetParLimits(1,0.0001,0.5);
	myBW1->SetParLimits(2,0.1,1000000);
	myBW1->SetParLimits(3,1,2);
	myBW1->SetParLimits(4,0.1,1);
	rhoHist->Fit("myBW1","RLQ");

	c1->cd(6);
	TH1F * chiHist = drawHists(B0Signal, "K_10_K_S0_M", cut, "M(K_{S}^{0}) [GeV]",0.45,0.55,mccut);
	TF1 * myBW = new TF1("myBW","gaus(0)+gaus(3)",0.45,0.55);
	myBW->SetParLimits(0,10,100000);
	myBW->SetParLimits(1,0.49,0.51);
	myBW->SetParLimits(2,0.0001,0.5);
	myBW->SetParLimits(3,10,10000);
	myBW->SetParLimits(4,0.48,0.52);
	myBW->SetParLimits(5,0.001,0.1);
	chiHist->Fit("myBW","L");
	c1->cd(7);
	string ksrho = "B0_K_10_K_S0_Rho";
	drawHists(B0Signal, "K_10_K_S0_Rho", cut, "#rho [cm]",0,10,mccut);
	
	c1->cd(8);
	drawHists(B0Signal, "VtxPvalue", cut, "p-value",0,1,mccut);
	//----------------------------------------------------------------//
	//----------------------------------------------------------------//
	TCanvas * c2 = new TCanvas("c2", "Dalitz", 0, 0, 1000, 500);
	c2->Divide(2,1);
	c2->cd(1);
	TH1F * MpipiRhoHist = new TH1F("MpipiRhoHist", ";M [GeV]", 100,0.2,1.4);
	B0Signal->Project("MpipiRhoHist","B0_m12",(mccut+mccutrho+cut).c_str());
	makePretty(MpipiRhoHist, kBlue+2);
	drawHists(B0Signal, "m12", cut, "M(#pi #pi) [GeV]", 0.2,1.4);
	MpipiRhoHist->Draw("same");
	c2->cd(2);
	TH1F * MKpiRhoHist = new TH1F("MKpiRhoHist", ";M [GeV]", 100,0.6,1.6);
	B0Signal->Project("MKpiRhoHist","B0_m23",(mccut+mccutrho+cut).c_str());
	makePretty(MKpiRhoHist, kBlue+2);
	drawHists(B0Signal, "m23", cut, "M(K#pi) [GeV]", 0.6,1.6);
	MKpiRhoHist->Draw("same");
}
