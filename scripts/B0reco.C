void B0reco(string filename = "test.root")
{
	string l0 =  "sqrt(B0_K_10_pi0__z0*B0_K_10_pi0__z0 + B0_K_10_pi0__d0*B0_K_10_pi0__d0) / abs(B0_K_10_pi0_cosTheta)";
	string l1 =  "sqrt(B0_K_10_pi1__z0*B0_K_10_pi1__z0 + B0_K_10_pi1__d0*B0_K_10_pi1__d0) / abs(B0_K_10_pi1_cosTheta)";
	TCanvas * c1 = new TCanvas("c1", "Dalitz",0,0, 1500,1000);
	string chi2 = "pow((B0_deltae -0.0524)/0.0977,2) + pow((B0_mbc-5.277)/0.0128,2) + pow((B0_cosTheta - 0.966)/0.0263,2) +  pow((B0_K_10_K_S0_M - 0.5)/0.005,2)";
	int nbins = 150;	
	c1->Divide(3,2);
	c1->cd(1);
	TFile * file = TFile::Open(filename.c_str());
	TTree* B0Signal = (TTree*)file->Get("B0Signal");
	string cut = "B0_mbc > 5.27 && B0_deltae > -0.2 && B0_deltae < 0.1"; // BEST
	//string cut = "B0_mbc > 5.2 && B0_deltae > -0.2 && B0_deltae < 0.2"; // S Akar
	//cut += " && B0_K_10_pi0_nPXDHits > 0 &&  B0_K_10_pi1_nPXDHits > 0";
	cut += " && B0_K_10_M < 1.8 ";
	cut += " && abs(B0_K_10_K_S0_M - 0.497) < 0.01 ";
	cut += " && B0_m12 > 0.6 && B0_m12 < 0.9";
	//cut += " && (B0_m23 > 0.95 ||  B0_m23 < 0.85) && (B0_m13 > 0.95 ||  B0_m13 < 0.85)";
	cut += " && abs(B0_K_10_K_S0_M - 0.497)/B0_K_10_K_S0_ErrM < 3 ";
	cut += " && B0_K_10_K_S0_Rho > 0.15";
	string angleCMS = "B0_K_10_useCMSFrame__bodaughterAngleInBetween__bo0__cm1__bc__bc";
	//cut += "&& " + angleCMS + " < 1.75";
	string mccut= "(B0_isSignal || B0_mcErrors == 258) &&";
	string mccutrho = " abs(B0_K_10_pi0_MC_MOTHER_ID) ==113 &&";
	//string mccut= "abs(B0_mcPDG) ==511 &&";
	//string cut = "1";
	//string cut = chi2 + "<8";
	int allB = B0Signal->Draw("B0_mcPDG", ("abs(B0_mcPDG) > -1 &&"+cut).c_str());
	float trueB = B0Signal->Draw("B0_mcPDG",(mccut+cut).c_str());
	int totalEntries = 1000;
	TH1F * deltaEHist = new TH1F("deltaEHist", ";axis []", 50,-0.5,0.5);
	TH1F * deltaEAllHist = new TH1F("deltaEAllHist", ";#Delta E [GeV]", 50,-0.5,0.5);
	
	TH1F * MHist = new TH1F("MHist", ";axis []", 50,5,5.5);
	TH1F * MAllHist = new TH1F("MAllHist", ";M [GeV]", 50,5,5.5);
	
	TH1F * MbcHist = new TH1F("MbcHist", ";M_{bc} []", 50,5.25,5.3);
	TH1F * MbcAllHist = new TH1F("MbcAllHist", ";M_{bc} [GeV]", 50,5.25,5.3);
	
	TH1F * cosHist = new TH1F("cosHist", ";cos#theta []", 50,0.8,1);
	TH1F * cosAllHist = new TH1F("cosAllHist", ";cos#theta []", 50,0.8,1);
	
	TH1F * rhoHist = new TH1F("rhoHist", ";#rho [cm]", nbins,0.8,2.);
	TH1F * rhoAllHist = new TH1F("rhoAllHist", ";M(K_{1}^{0}) [GeV]", nbins,0.8,2.);
	
	TH1F * chiHist = new TH1F("chiHist", ";M(K_{S}^{0}) [GeV]", nbins,0.45,0.550);
	TH1F * chiAllHist = new TH1F("chiAllHist", ";M(K_{S}^{0}) [GeV]", nbins,0.45,0.550);
	cout << "All B0: " << allB << "; true B0: " << trueB << "\n"; 
	cout << "Purity B0: " << trueB/allB*100 << "%\n";
	B0Signal->Project("deltaEAllHist","B0_deltae",cut.c_str());
	B0Signal->Project("deltaEHist","B0_deltae",(mccut + cut).c_str(), "same");
	deltaEAllHist->Draw();
	makePretty(deltaEHist);
	makePretty(deltaEAllHist, kGray+1);
	deltaEHist->Draw("same");
	c1->cd(2);
	B0Signal->Project("MAllHist","B0_M",cut.c_str());
	B0Signal->Project("MHist","B0_M",(mccut+cut).c_str(), "same");
	MAllHist->Draw();
	makePretty(MHist);
	makePretty(MAllHist, kGray+1);
	MHist->Draw("same");
	c1->cd(3);
	B0Signal->Project("MbcAllHist","B0_mbc",cut.c_str());
	B0Signal->Project("MbcHist","B0_mbc",(mccut+cut).c_str(), "same");
	MbcAllHist->Draw();
	makePretty(MbcHist);
	makePretty(MbcAllHist, kGray+1);
	MbcHist->Draw("same");
	
	c1->cd(4);
	B0Signal->Project("cosAllHist","B0_cosTheta",cut.c_str());
	B0Signal->Project("cosHist","B0_cosTheta",(mccut+cut).c_str(), "same");
	cosAllHist->Draw();
	makePretty(cosHist);
	makePretty(cosAllHist, kGray+1);
	cosHist->Draw("same");
	
	c1->cd(5);
	//string rho = "B0_Rho";
	//string rho = "B0_K_10_K_S0_M";
	string rho = "B0_K_10_M";
	B0Signal->Project("rhoAllHist",rho.c_str(),cut.c_str());
	B0Signal->Project("rhoHist",rho.c_str(),(mccut+cut).c_str(), "same");
	rhoAllHist->Draw();
	makePretty(rhoHist);
	makePretty(rhoAllHist, kGray+1);
	rhoHist->Draw("same");
	TF1 * myBW1 = new TF1("myBW1","TMath::BreitWigner(x,[0],[1])*[2]*TMath::Gaus(x,[3],[4])",1,2.);

	myBW1->SetParLimits(0,1.2,1.4);
	myBW1->SetParLimits(1,0.0001,0.1);
	myBW1->SetParLimits(2,0.1,100000);
	myBW1->SetParLimits(3,1,2);
	myBW1->SetParLimits(4,0.1,1);
	rhoHist->Fit("myBW1","RLQ");
	c1->cd(6);
	string k0mass = "B0_K_10_K_S0_M";
	B0Signal->Project("chiAllHist",k0mass.c_str(),cut.c_str());
	B0Signal->Project("chiHist",k0mass.c_str(),(mccut+cut).c_str(), "same");
	chiAllHist->Draw();
	makePretty(chiHist);
	makePretty(chiAllHist, kGray+1);
	chiHist->Draw("same");
	TF1 * myBW = new TF1("myBW","gaus(0)+gaus(3)",0.45,0.55);
	myBW->SetParLimits(0,10,10000);
	myBW->SetParLimits(1,0.49,0.51);
	myBW->SetParLimits(2,0.0001,0.01);
	myBW->SetParLimits(3,10,1000);
	myBW->SetParLimits(4,0.48,0.52);
	myBW->SetParLimits(5,0.001,1);
	chiHist->Fit("myBW","L");
	TCanvas * c2 = new TCanvas("c2", "Dalitz", 0, 0, 1000, 500);
	c2->Divide(2,1);
	c2->cd(1);
	TH1F * MpipiHist = new TH1F("MpipiHist", ";axis []", nbins,0.2,1.4);
	TH1F * MpipiAllHist = new TH1F("MpipiAllHist", ";M [GeV]", nbins,0.2,1.4);
	TH1F * MpipiRhoHist = new TH1F("MpipiRhoHist", ";M [GeV]", nbins,0.2,1.4);
	B0Signal->Project("MpipiAllHist","B0_m12",cut.c_str());
	B0Signal->Project("MpipiRhoHist","B0_m12",(mccut+mccutrho+cut).c_str());
	B0Signal->Project("MpipiHist","B0_m12",(mccut+cut).c_str(), "same");
	MpipiAllHist->Draw();
	makePretty(MpipiHist);
	makePretty(MpipiAllHist, kGray+1);
	makePretty(MpipiRhoHist, kBlue+2);
	MpipiHist->Draw("same");
	MpipiRhoHist->Draw("same");
	c2->cd(2);
	TH1F * MKpiHist = new TH1F("MKpiHist", ";axis []", nbins,0.6,1.6);
	TH1F * MKpiAllHist = new TH1F("MKpiAllHist", ";M [GeV]", nbins,0.6,1.6);
	TH1F * MKpiRhoHist = new TH1F("MKpiRhoHist", ";M [GeV]", nbins,0.6,1.6);
	B0Signal->Project("MKpiAllHist","B0_m23",cut.c_str());
	B0Signal->Project("MKpiRhoHist","B0_m23",(mccut+mccutrho+cut).c_str());
	B0Signal->Project("MKpiHist","B0_m23",(mccut+cut).c_str(), "same");
	MKpiAllHist->Draw();
	makePretty(MKpiHist);
	makePretty(MKpiAllHist, kGray+1);
	makePretty(MKpiRhoHist, kBlue+2);
	MKpiHist->Draw("same");
	MKpiRhoHist->Draw("same");
	
	
	/*B0Signal->Draw("B0_Rho",cut.c_str());
	B0Signal->Draw("B0_Rho",("abs(B0_mcPDG) == 511&&"+cut).c_str(), "same");
	B0Signal->Draw("abs(B0_K_10_pi0__d0 - B0_K_10_pi1__d0)",cut.c_str());
	B0Signal->Draw("abs(B0_K_10_pi0__d0 - B0_K_10_pi1__d0)",("abs(B0_mcPDG) == 511&&"+cut).c_str(), "same");
	B0Signal->Draw("sqrt(B0_K_10_pi0__z0*B0_K_10_pi0__z0 + B0_K_10_pi0__d0*B0_K_10_pi0__d0) / abs(B0_K_10_pi0_cosTheta)",cut.c_str());
	B0Signal->Draw("sqrt(B0_K_10_pi0__z0*B0_K_10_pi0__z0 + B0_K_10_pi0__d0*B0_K_10_pi0__d0) / abs(B0_K_10_pi0_cosTheta)",("abs(B0_mcPDG) == 511&&"+cut).c_str(), "same");
	B0Signal->Draw((l0 + "-" + l1).c_str(),cut.c_str());
	B0Signal->Draw((l0 + "-" + l1).c_str(),("abs(B0_mcPDG) == 511&&"+cut).c_str(), "same");
	*/

	//B0Signal->Draw("(B0_K_10_pi0_cosTheta):(B0_K_10_pi1_cosTheta)",cut.c_str());
	//B0Signal->Draw("(B0_K_10_pi0_cosTheta):(B0_K_10_pi1_cosTheta)",("abs(B0_mcPDG) == 511&&"+cut).c_str(), "same");
}
