TH1F *  addBkg(THStack * stackmbc, THStack * stackde, THStack * stackmva, string continuumname, vector<string> cuts, float scale, int color, int style, int nbins = 50)
{
	float mvaup = 0.6;
	int nbinsde = 20;
	string cut = cuts[0];
	string mbccut = cuts[1];
	string decut = cuts[2];
	string mvacut = cuts[3];
	TFile * fileBkg = TFile::Open(continuumname.c_str());
	TTree* B0Continuum = (TTree*)fileBkg->Get("B0Signal");
	TH1F * dEqqHist = new TH1F("dEqqHist", ";#Delta E [GeV]", nbinsde,-0.2,0.2);
	TH1F * MbcqqHist = new TH1F("MbcqqHist", ";M_{bc} [GeV]", nbins,5.2,5.3);
	TH1F * MVAqqHist = new TH1F("MVAqqHist", ";MVA", nbins,0,mvaup);
	B0Continuum->Project("dEqqHist", "B0_deltae", ( cut + mvacut + mbccut).c_str());
	B0Continuum->Project("MbcqqHist", "B0_mbc", ( cut + mvacut + decut).c_str());
	B0Continuum->Project("MVAqqHist", "B0_CSMVA", ( cut + mbccut + decut).c_str());
	makePretty(dEqqHist, color,style, color-1);
	makePretty(MbcqqHist, color,style,color-1);
	makePretty(MVAqqHist, color,style,color-1);
	//dEqqHist->Scale(scale);
	//MbcqqHist->Scale(scale);
	//MVAqqHist->Scale(scale);
	stackmbc->Add(MbcqqHist);
	stackde->Add(dEqqHist);
	stackmva->Add(MVAqqHist);
	return dEqqHist;
}

void checkVariables(string signalname = "mixed/lumi555fb.root", 
			   string lightname = "light/lumi555fb.root", 
			   string ccbarname = "ccbar/lumi555fb.root", 
			   string chargedname = "charged/lumi555fb.root", 
			   bool signalEnhanced = 0, string Kres = "Xsd")
{
	float signalK1Lumi = 0.7; // ab-1
	float ccbarLumi = 0.7; // ab-1
	float uubarLumi = 0.7; // ab-1
	float ssbarLumi = 0.7; // ab-1
	float ddbarLumi = 0.7; // ab-1
	float bcharLumi = 0.7; // ab-1
	float bneutLumi = 0.7; // ab-1
	float minLumi = 0.7;
	
	float fsignal = 1.0; 
	float fccbar = 1.0;
	TCanvas * c1 = new TCanvas("c1", "Dalitz",0,0, 1500,500);
	c1->Divide(3,1);
	c1->cd(1);
	int nbins = 50;
	int nbinsde = 20;
	TH1F * dEqqHist = NULL;
	TH1F * MbcqqHist = NULL;
	TH1F * MVAqqHist = NULL;
	
	string cut = getBasicCuts(1,Kres);
	//cut += " && B0_gamma_MC_MOTHER_ID != 111 && B0_gamma_MC_MOTHER_ID != 221";
	//string trueB = "&& (abs(B0_K_10_mcPDG) == 30343 && abs(B0_gamma_MC_MOTHER_ID) == 511)";
	string trueB = "&& B0_isSignal";
	//string scfB = "&& B0_isSignal == 0 && B0_isContinuumEvent == 0 ";
	//string xsgBkg = "&& B0_isContinuumEvent == 0 && !(abs(B0_K_10_mcPDG) == 30343)";
	string xsgBkg = "&& B0_isContinuumEvent == 0 && B0_isSignal == 0";
	string scfB = "&& B0_isSignal == 0 && B0_isContinuumEvent == 0 && (B0_mcErrors == 306 || B0_mcErrors == 512)";
	//string xsgBkg = "&& B0_isSignal == 0 && B0_isContinuumEvent == 0 && B0_mcErrors != 306 && B0_mcErrors != 512";
	string continuum = "&& B0_isContinuumEvent == 1";
	
	string decut = "&& B0_deltae > -0.15 && B0_deltae < 0.1";
	string mbccut = "&& B0_mbc > 5.27";
	string mvacut = "&&1";//"&& B0_CSMVA > 0.3";
	if (!signalEnhanced) 
	{
		decut = "&& 1";
		mbccut = "&& 1";
		mvacut = "&& 1";
	}
	vector<string> cuts = {cut, mbccut, decut, mvacut};
	  /////////////////////////////////////////
	 //	    SIGNAL+SCF+XsGamma	        //
	/////////////////////////////////////////	
	float mvaup = 0.6;
	TFile * fileSig = TFile::Open(signalname.c_str());
	fileSig->cd();
	TTree* B0Signal = (TTree*)fileSig->Get("B0Signal");
	TH1F * dEHist = new TH1F("dEHist", ";#Delta E [GeV]", nbinsde,-0.2,0.2);
	TH1F * dEscfHist = new TH1F("dEscfHist", ";#Delta E [GeV]", nbinsde,-0.2,0.2);
	TH1F * dEXsGHist = new TH1F("dEXsGHist", ";#Delta E [GeV]", nbinsde,-0.2,0.2);
	
	TH1F * MbcHist = new TH1F("MbcHist", ";M_{bc} [GeV]", nbins,5.2,5.3);
	TH1F * MbcscfHist = new TH1F("MbcscfHist", ";M_{bc} [GeV]", nbins,5.2,5.3);
	TH1F * MbcXsGHist = new TH1F("MbcXsGHist", ";M_{bc} [GeV]", nbins,5.2,5.3);
	
	TH1F * MVAHist = new TH1F("MVAHist", ";MVA", nbins,0,mvaup);
	TH1F * MVAscfHist = new TH1F("MVAscfHist", ";MVA", nbins,0,mvaup);
	TH1F * MVAXsGHist = new TH1F("MVAXsGHist", ";MVA", nbins,0,mvaup);
	B0Signal->Project("dEMbcHist", "B0_mbc : B0_deltae", ( cut ).c_str());
	int nsignal = B0Signal->Project("dEHist", "B0_deltae", ( cut + mvacut + mbccut + trueB ).c_str());
	B0Signal->Project("MbcHist", "B0_mbc", ( cut + decut + mvacut + trueB ).c_str());
	B0Signal->Project("dEscfHist", "B0_deltae", ( cut + mbccut + mvacut + scfB ).c_str());
	int nmixed = B0Signal->Project("dEXsGHist", "B0_deltae", ( cut + mbccut + mvacut + xsgBkg ).c_str());
	B0Signal->Project("MbcscfHist", "B0_mbc", ( cut + decut + mvacut + scfB ).c_str());
	B0Signal->Project("MbcXsGHist", "B0_mbc", ( cut + decut + mvacut + xsgBkg ).c_str());
	B0Signal->Project("MVAscfHist", "B0_CSMVA", ( cut + decut + mbccut + scfB ).c_str());
	B0Signal->Project("MVAXsGHist", "B0_CSMVA", ( cut + decut + mbccut + xsgBkg ).c_str());
	B0Signal->Project("MVAHist", "B0_CSMVA", ( cut + decut + mbccut + trueB ).c_str());
	/*dEHist->Scale(fsignal);
	dEscfHist->Scale(fsignal);
	dEXsGHist->Scale(fsignal);
	MbcHist->Scale(fsignal);
	MbcscfHist->Scale(fsignal);
	MbcXsGHist->Scale(fsignal);
	MVAHist->Scale(fsignal);
	MVAscfHist->Scale(fsignal);
	MVAXsGHist->Scale(fsignal);*/
	
	makePretty(dEHist, kBlue, 3004, kWhite);
	makePretty(MbcHist, kBlue, 3004, kWhite);
	makePretty(MVAHist, kBlue, 3004, kWhite);
	
	makePretty(dEscfHist,kBlue+1);
	makePretty(MbcscfHist,kBlue+1);
	makePretty(MVAscfHist,kBlue+1);
	
	makePretty(MVAXsGHist,kRed-4, 1001, kRed-6);
	makePretty(dEXsGHist,kRed-4, 1001, kRed-6);
	makePretty(MbcXsGHist,kRed-4, 1001, kRed-6);
	  /////////////////////////////////////////
	 //		CONTINUUM		//
	/////////////////////////////////////////	
	
	THStack * stackmbc = new THStack("stackmbc","stackde");
	stackmbc->Add(MbcXsGHist);

	THStack * stackde = new THStack("stackde","");
	stackde->Add(dEXsGHist);
	
	THStack * stackmva = new THStack("stackmva","");
	MVAXsGHist->SetMinimum(0.0001);
	stackmva->Add(MVAXsGHist);
	
	TH1F * chargeHist = addBkg(stackmbc, stackde, stackmva, chargedname, cuts, fccbar, kMagenta-5, 1001, nbins);
	TH1F * lightHist = addBkg(stackmbc, stackde, stackmva, lightname, cuts, fccbar, kCyan-5, 1001, nbins);
	TH1F * ccbarHist = addBkg(stackmbc, stackde, stackmva, ccbarname, cuts, fccbar, kCyan-3, 1001, nbins);
	std::cout << "N signal: " << nsignal << std::endl;
	std::cout << "N charged: " << chargeHist->GetEntries() << std::endl;
	std::cout << "N mixed: " << nmixed << std::endl;
	std::cout << "N ccbar: " << lightHist->GetEntries() << std::endl;
	std::cout << "N light: " << ccbarHist->GetEntries() << std::endl;

	stackmbc->Add(MbcHist);
	stackmva->Add(MVAHist);
	stackde->Add(dEHist);
	//stackmbc->Add(MbcscfHist);
	//stackmva->Add(MVAscfHist);
	//stackde->Add(dEscfHist);
	//stackmbc->Add(MbcqqHist);
	stackmbc->Draw("he");
	stackmbc->SetMaximum( stackmbc->GetMaximum()*1.15 );
	TLegend *legendMean2 = new TLegend(0.15,0.55,0.5,0.85,NULL,"brNDC");
	legendMean2->SetFillStyle(0);
	legendMean2->SetBorderSize(0);
	legendMean2->AddEntry(MbcHist,"Signal","f");
	//legendMean2->AddEntry(MbcscfHist,"SCF","f");
	legendMean2->SetHeader("L = 550 fb^{-1}", "C");
	legendMean2->AddEntry(ccbarHist,"c#bar{c}","f");
	legendMean2->AddEntry(lightHist,"u#bar{u}+d#bar{d}+s#bar{s}","f");
	legendMean2->AddEntry(chargeHist,"B^{+}#bar{B}^{-}","f");
	legendMean2->AddEntry(MbcXsGHist,"B^{0}#bar{B}^{0}","f");
	legendMean2->Draw();
	stackmbc->SetTitle(";M_{bc} [GeV]");
	stackmbc->GetXaxis()->SetLabelSize(0.04);
	stackmbc->GetYaxis()->SetLabelSize(0.04);
	c1->cd(2);

	stackde->Draw("he");
	stackde->SetTitle(";#Delta E [GeV]");
	stackde->GetXaxis()->SetLabelSize(0.04);
	stackde->GetYaxis()->SetLabelSize(0.04);
	stackde->SetMaximum( stackde->GetMaximum()*1.15 );
	c1->cd(3);
	gPad->SetLogy();
	stackmva->Draw("he");
	stackmva->SetTitle(";MVA score");
	stackmva->GetXaxis()->SetLabelSize(0.04);
	stackmva->GetYaxis()->SetLabelSize(0.04);
	//[0]-sqrt((x*x*x+[2]*x+[3]*x+[4]))*[1]
	//[0]*x*sqrt(1-x*x/[1]/[1])*exp(-[2]*sqrt(1-x*x/[1]/[1]))
	//TH2F * dEMbcHist = new TH2F("dEMbcHist", "; #Delta E [GeV];M_{bc} [GeV]", nbins,-0.2,0.2, nbins,5.2,5.3);
	//dEMbcHist->Draw("colz");
}
