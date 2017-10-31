void anglesRho(string filename = "test.root")
{
	TCanvas * c1 = new TCanvas("c1", "Dalitz",0,0, 1000,1000);
	c1->Divide(2,2);
	c1->cd(1);
	TFile * file = TFile::Open(filename.c_str());
	TTree* B0Signal = (TTree*)file->Get("B0Signal");
	string cut = "B0_mbc > 5.27 && B0_deltae > -0.2 && B0_deltae < 0.1"; // BEST
	cut += " && B0_K_10_M < 1.8 ";
	cut += " && abs(B0_K_10_K_S0_M - 0.497) < 0.01 ";
	cut += " && B0_m12 > 0.6 ";
	cut += " && (B0_m23 > 0.95 ||  B0_m23 < 0.85) && (B0_m13 > 0.95 ||  B0_m13 < 0.85)";
	//cut += " && abs(B0_K_10_K_S0_M - 0.497)/B0_K_10_K_S0_ErrM < 3 ";
	string mccut= "(B0_isSignal || B0_mcErrors == 258) &&";
	string mccutrho = " abs(B0_K_10_pi0_MC_MOTHER_ID) ==113 &&";
	string pi0x = "B0_K_10_pi0_P4[0]";
	string pi0y = "B0_K_10_pi0_P4[1]";
	string pi0z = "B0_K_10_pi0_P4[2]";
	
	string pi1x = "B0_K_10_pi1_P4[0]";
	string pi1y = "B0_K_10_pi1_P4[1]";
	string pi1z = "B0_K_10_pi1_P4[2]";
	
	string Kx = "B0_K_10_K_S0_P4[0]";
	string Ky = "B0_K_10_K_S0_P4[1]";
	string Kz = "B0_K_10_K_S0_P4[2]";
	
	string pi0p = "B0_K_10_pi0_P";
	string pi1p = "B0_K_10_pi1_P";
	string pipip =  "sqrt(pow(" + pi0x + " + " + pi1x + ",2) + " + "pow(" + pi0y + " + " + pi1y + ",2) + " +"pow(" + pi0z + " + " + pi1z + ",2))";
	string Kp = "B0_K_10_K_S0_P";
	string helangle = "B0_K_10_cosHelicityAngle";
	std::cout << pipip << std::endl;

	string angleBtw = "(" + pi0x + "*" + pi1x + " + " + pi0y + "*" + pi1y + " + "+ pi0z + "*" + pi1z + ")/" + pi0p +"/"+pi1p;
	string angleCMS = "B0_K_10_useCMSFrame__bodaughterAngleInBetween__bo0__cm1__bc__bc";
	//cut += "&& " + angleCMS + " < 1.75";
	string angleKpipi = "((" + pi0x + "+" + pi1x + ")*" + Kx + " + (" + pi0y + "+" + pi1y + ")*" + Ky + " + ("+ pi0z + "+" + pi1z + ")*" + Kz +")/" + pipip +"/"+Kp;
	std::cout << angleKpipi << std::endl;
	int allB = B0Signal->Draw(pi0z.c_str(), (mccut+cut).c_str());
	TH1F * pipiAngleHist = new TH1F("pipiAngleHist", ";cos#alpha []", 50,-1,1);
	TH1F * pipiAngleRhoHist = new TH1F("pipiAngleRhoHist", ";cos#alpha []", 50,-1,1);
	TH1F * pipiAngleAllHist = new TH1F("pipiAngleAllHist", ";cos#alpha []", 50,-1,1);
	B0Signal->Project("pipiAngleHist",angleBtw.c_str(), (mccut+cut).c_str());
	B0Signal->Project("pipiAngleAllHist",angleBtw.c_str(), (cut).c_str());
	B0Signal->Project("pipiAngleRhoHist",angleBtw.c_str(), (mccut+mccutrho+cut).c_str());
	makePretty(pipiAngleHist);
	makePretty(pipiAngleAllHist,kGray+1);
	makePretty(pipiAngleRhoHist,kBlue+2);
	pipiAngleAllHist->Draw();
	pipiAngleHist->Draw("same");
	pipiAngleRhoHist->Draw("same");
	c1->cd(2);
	TH1F * KpipiAngleHist = new TH1F("KpipiAngleHist", ";cos#alpha []", 50,0.5,1);
	TH1F * KpipiAngleRhoHist = new TH1F("KpipiAngleRhoHist", ";cos#alpha []", 50,0.5,1);
	TH1F * KpipiAngleAllHist = new TH1F("KpipiAngleAllHist", ";cos#alpha []", 50,0.5,1);
	B0Signal->Project("KpipiAngleHist",angleKpipi.c_str(), (mccut+cut).c_str());
	B0Signal->Project("KpipiAngleAllHist",angleKpipi.c_str(), (cut).c_str());
	B0Signal->Project("KpipiAngleRhoHist",angleKpipi.c_str(), (mccut+mccutrho+cut).c_str());
	makePretty(KpipiAngleHist);
	makePretty(KpipiAngleAllHist,kGray+1);
	makePretty(KpipiAngleRhoHist,kBlue+2);
	KpipiAngleAllHist->Draw();
	KpipiAngleHist->Draw("same");
	KpipiAngleRhoHist->Draw("same");
	c1->cd(3);
	TH1F * HelAngleHist = new TH1F("HelAngleHist", ";cos#alpha []", 50,-1,1);
	TH1F * HelAngleRhoHist = new TH1F("HelAngleRhoHist", ";cos#alpha []", 50,-1,1);
	TH1F * HelAngleAllHist = new TH1F("HelAngleAllHist", ";cos#alpha []", 50,-1,1);
	B0Signal->Project("HelAngleHist",helangle.c_str(), (mccut+cut).c_str());
	B0Signal->Project("HelAngleAllHist",helangle.c_str(), (cut).c_str());
	B0Signal->Project("HelAngleRhoHist",helangle.c_str(), (mccut+mccutrho+cut).c_str());
	makePretty(HelAngleHist);
	makePretty(HelAngleAllHist,kGray+1);
	makePretty(HelAngleRhoHist,kBlue+2);
	HelAngleAllHist->SetMinimum(0);
	HelAngleAllHist->Draw();
	HelAngleHist->Draw("same");
	HelAngleRhoHist->Draw("same");

	c1->cd(4);
	TH1F * cmsAngleHist = new TH1F("cmsAngleHist", ";cos#alpha []", 50,0,3.14);
	TH1F * cmsAngleRhoHist = new TH1F("cmsAngleRhoHist", ";cos#alpha []", 50,0,3.14);
	TH1F * cmsAngleAllHist = new TH1F("cmsAngleAllHist", ";cos#alpha []", 50,0,3.14);
	B0Signal->Project("cmsAngleHist",angleCMS.c_str(), (mccut+cut).c_str());
	B0Signal->Project("cmsAngleAllHist",angleCMS.c_str(), (cut).c_str());
	B0Signal->Project("cmsAngleRhoHist",angleCMS.c_str(), (mccut+mccutrho+cut).c_str());
	makePretty(cmsAngleHist);
	makePretty(cmsAngleAllHist,kGray+1);
	makePretty(cmsAngleRhoHist,kBlue+2);
	cmsAngleAllHist->SetMinimum(0);
	cmsAngleAllHist->Draw();
	cmsAngleHist->Draw("same");
	cmsAngleRhoHist->Draw("same");





}

