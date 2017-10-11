
void makePretty(TH1F* htemp, int color = kBlue)
{
	htemp->SetLineWidth(3);
	htemp->SetFillStyle(3004);
	htemp->SetFillColor(color);
	htemp->SetLineColor(color);
	if(color != kBlue)
	{
		htemp->SetFillStyle(3005);
	}
}
void B0reco(string filename = "test.root")
{
	string l0 =  "sqrt(B0_K_10_pi0__z0*B0_K_10_pi0__z0 + B0_K_10_pi0__d0*B0_K_10_pi0__d0) / abs(B0_K_10_pi0_cosTheta)";
	string l1 =  "sqrt(B0_K_10_pi1__z0*B0_K_10_pi1__z0 + B0_K_10_pi1__d0*B0_K_10_pi1__d0) / abs(B0_K_10_pi1_cosTheta)";
	TCanvas * c1 = new TCanvas("c1", "Dalitz",0,0, 1500,1000);
	string chi2 = "pow((B0_deltae -0.0524)/0.0977,2) + pow((B0_mbc-5.277)/0.0128,2) + pow((B0_cosTheta - 0.966)/0.0263,2) +  pow((B0_K_10_K_S0_M - 0.5)/0.005,2)";
	
	c1->Divide(3,2);
	c1->cd(1);
	TFile * file = TFile::Open(filename.c_str());
	TTree* B0Signal = (TTree*)file->Get("B0Signal");
	//string cut = "abs(B0_K_10_pi0__d0 - B0_K_10_pi1__d0) < 0.04";//"B0_M > 5.1 && B0_M< 5.35 && B0_deltae < 0.18 && B0_deltae > -0.2 && B0_mbc > 5.275 &&B0_cosTheta > 0.92";//"B0_K_10_pi0_Reconstructible";//"B0_K_10_pi0_SeenInSVD == 1 && B0_K_10_pi1_SeenInSVD == 1 &&B0_K_10_pi0_SeenInPXD == 1 && B0_K_10_pi1_SeenInPXD";
	//string cut = "abs("+ (l0 + "-" + l1 + ") < 0.5") ;
	//string cut = "B0_mbc > 5.27 && B0_M < 5.4 && abs(B0_deltae) < 0.25 && B0_cosTheta > 0.9";
	string cut = "B0_mbc > 5.27 && B0_deltae > -0.2 && B0_deltae < 0.1"; // BEST
	//string cut = "B0_mbc < 5.292 && B0_mbc > 5.2 && B0_K_10_K_S0_M > 0.45 && B0_K_10_K_S0_M < 5.4 && abs(B0_deltae) < 0.2";
	//string cut = "1";
	//string cut = chi2 + "<8";
	int allB = B0Signal->Draw("B0_mcPDG", ("abs(B0_mcPDG) > -1 &&"+cut).c_str());
	float trueB = B0Signal->Draw("B0_mcPDG",("abs(B0_mcPDG) == 511 &&"+cut).c_str());
	int totalEntries = 1000;
	TH1F * deltaEHist = new TH1F("deltaEHist", ";axis []", 50,-0.5,0.5);
	TH1F * deltaEAllHist = new TH1F("deltaEAllHist", ";#Delta E [GeV]", 50,-0.5,0.5);
	
	TH1F * MHist = new TH1F("MHist", ";axis []", 50,5,5.5);
	TH1F * MAllHist = new TH1F("MAllHist", ";M [GeV]", 50,5,5.5);
	
	TH1F * MbcHist = new TH1F("MbcHist", ";M_{bc} []", 50,5.2,5.35);
	TH1F * MbcAllHist = new TH1F("MbcAllHist", ";M_{bc} [GeV]", 50,5.2,5.35);
	
	TH1F * cosHist = new TH1F("cosHist", ";cos#theta []", 50,0.8,1);
	TH1F * cosAllHist = new TH1F("cosAllHist", ";cos#theta []", 50,0.8,1);
	
	TH1F * rhoHist = new TH1F("rhoHist", ";#rho [cm]", 50,0.6,2.20);
	TH1F * rhoAllHist = new TH1F("rhoAllHist", ";#rho [cm]", 50,0.6,2.20);
	
	TH1F * chiHist = new TH1F("chiHist", ";M(K_{S}^{0}) [GeV]", 50,0.45,0.550);
	TH1F * chiAllHist = new TH1F("chiAllHist", ";M(K_{S}^{0}) [GeV]", 50,0.45,0.550);
	cout << "All B0: " << allB << "; true B0: " << trueB << "\n"; 
	cout << "Purity B0: " << trueB/allB*100 << "%\n";
	B0Signal->Project("deltaEAllHist","B0_deltae",cut.c_str());
	B0Signal->Project("deltaEHist","B0_deltae",("abs(B0_mcPDG) == 511 &&" + cut).c_str(), "same");
	deltaEAllHist->Draw();
	makePretty(deltaEHist);
	makePretty(deltaEAllHist, kGray+1);
	deltaEHist->Draw("same");
	c1->cd(2);
	B0Signal->Project("MAllHist","B0_M",cut.c_str());
	B0Signal->Project("MHist","B0_M",("abs(B0_mcPDG) == 511 &&"+cut).c_str(), "same");
	MAllHist->Draw();
	makePretty(MHist);
	makePretty(MAllHist, kGray+1);
	MHist->Draw("same");
	c1->cd(3);
	B0Signal->Project("MbcAllHist","B0_mbc",cut.c_str());
	B0Signal->Project("MbcHist","B0_mbc",("abs(B0_mcPDG) == 511&&"+cut).c_str(), "same");
	MbcAllHist->Draw();
	makePretty(MbcHist);
	makePretty(MbcAllHist, kGray+1);
	MbcHist->Draw("same");
	
	c1->cd(4);
	B0Signal->Project("cosAllHist","B0_cosTheta",cut.c_str());
	B0Signal->Project("cosHist","B0_cosTheta",("abs(B0_mcPDG) == 511&&"+cut).c_str(), "same");
	cosAllHist->Draw();
	makePretty(cosHist);
	makePretty(cosAllHist, kGray+1);
	cosHist->Draw("same");
	
	c1->cd(5);
	//string rho = "B0_Rho";
	//string rho = "B0_K_10_K_S0_M";
	string rho = "B0_K_10_M";
	B0Signal->Project("rhoAllHist",rho.c_str(),cut.c_str());
	B0Signal->Project("rhoHist",rho.c_str(),("abs(B0_mcPDG) == 511&&"+cut).c_str(), "same");
	rhoAllHist->Draw();
	makePretty(rhoHist);
	makePretty(rhoAllHist, kGray+1);
	rhoHist->Draw("same");
	TF1 * myBW1 = new TF1("myBW1","TMath::BreitWigner(x,[0],[1])*[2]*TMath::Gaus(x,[3],[4])",0.6,2.2);

	myBW1->SetParLimits(0,1.1,1.5);
	myBW1->SetParLimits(1,0.0001,0.1);
	myBW1->SetParLimits(2,0.1,1000);
	myBW1->SetParLimits(3,1,2);
	myBW1->SetParLimits(4,0.1,1);
	rhoHist->Fit("myBW1","RLQ");
	c1->cd(6);
	string k0mass = "B0_K_10_K_S0_M";
	B0Signal->Project("chiAllHist",k0mass.c_str(),cut.c_str());
	B0Signal->Project("chiHist",k0mass.c_str(),("abs(B0_mcPDG) == 511&&"+cut).c_str(), "same");
	chiAllHist->Draw();
	makePretty(chiHist);
	makePretty(chiAllHist, kGray+1);
	chiHist->Draw("same");
	TF1 * myBW = new TF1("myBW","TMath::BreitWigner(x,[0],[1])*[2]",0.45,0.55);
	myBW->SetParLimits(0,0.45,0.55);
	myBW->SetParLimits(1,0.0001,0.05);
	myBW->SetParLimits(2,0.1,1000);
	chiHist->Fit("myBW","RLQ");
	
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
