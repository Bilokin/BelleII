TH1F *  addBkg(THStack * stackmbc, THStack * stackde, THStack * stackmva,  THStack * stackdt, string continuumname, vector<string> cuts, float scale, int color, int style, int nbins = 50)
{
	string decutdt = "&& B0_deltae > -0.15 && B0_deltae < 0.1";
	string mbccutdt = "&& B0_mbc > 5.27";
	float mvaup = 2.;
	int nbinsde = 25;
	string cut = cuts[0];
	string mbccut = cuts[1];
	string decut = cuts[2];
	string mvacut = cuts[3];
	TFile * fileBkg = TFile::Open(continuumname.c_str());
	TTree* B0Continuum = (TTree*)fileBkg->Get("B0Signal");
	TH1F * dEqqHist = new TH1F("dEqqHist", ";#Delta E [GeV]", nbinsde,-0.2,0.2);
	TH1F * MbcqqHist = new TH1F("MbcqqHist", ";M_{bc} [GeV]", nbins,5.2,5.3);
	TH1F * MVAqqHist = new TH1F("MVAqqHist", ";MVA", nbins,0,mvaup);
	TH1F * dtqqHist = new TH1F("dtqqHist", ";MVA", nbins,-10,10);
	B0Continuum->Project("dEqqHist", "B0_deltae", ( cut + mvacut + mbccut).c_str());
	B0Continuum->Project("MbcqqHist", "B0_mbc", ( cut + mvacut + decut).c_str());
	B0Continuum->Project("MVAqqHist", "B0_CSMVA", ( cut + mbccut + decut).c_str());
	B0Continuum->Project("dtqqHist", "B0_DeltaT", ( cut + mbccutdt + decutdt).c_str());
	makePretty(dEqqHist, color,style, color-1);
	makePretty(MbcqqHist, color,style,color-1);
	makePretty(MVAqqHist, color,style,color-1);
	makePretty(dtqqHist, color,style,color-1);
	//dEqqHist->Scale(scale);
	//MbcqqHist->Scale(scale);
	//MVAqqHist->Scale(scale);
	stackmbc->Add(MbcqqHist);
	stackde->Add(dEqqHist);
	stackmva->Add(MVAqqHist);
	stackdt->Add(dtqqHist);
	return dEqqHist;
}

//void checkVariables(string signalname = "precut-veto3/mixed-gen2.root", 
//			   string lightname = "precut-veto3/light-lumi630fb-veto3.root", 
//			   string ccbarname = "precut-veto3/ccbar-lumi630fb-veto3.root", 
//			   string chargedname = "precut-veto3/charged-lumi630fb-veto3.root", 
void checkVariables(string signalname = "root/tmp-after-optimization-csmva-veto34.root", 
			   string lightname = "root/tmp-after-optimization-csmva-veto33.root", 
			   string ccbarname = "root/tmp-after-optimization-csmva-veto31.root", 
			   string chargedname = "root/tmp-after-optimization-csmva-veto32.root", 
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
	TCanvas * c1 = new TCanvas("c1", "Dalitz",0,0, 1000,1000);
	c1->Divide(2,2);
	c1->cd(1);
	int nbins = 50;
	int nbinsde = 25;
	TH1F * dEqqHist = NULL;
	TH1F * MbcqqHist = NULL;
	TH1F * MVAqqHist = NULL;
	
	string cut = getCuts(Kres);
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
	string decutdt = "&& B0_deltae > -0.15 && B0_deltae < 0.1";
	string mbccutdt = "&& B0_mbc > 5.27";
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
	float mvaup = 2.;
	TFile * fileSig = TFile::Open(signalname.c_str());
	fileSig->cd();
	TTree* B0Signal = (TTree*)fileSig->Get("B0Signal");
	TH1F * dEHist = new TH1F("dEHist", ";#Delta E [GeV]", nbinsde,-0.2,0.2);
	TH1F * dEscfHist = new TH1F("dEscfHist", ";#Delta E [GeV]", nbinsde,-0.2,0.2);
	TH1F * dEXsGHist = new TH1F("dEXsGHist", ";#Delta E [GeV]", nbinsde,-0.2,0.2);
	
	TH1F * MbcHist = new TH1F("MbcHist", ";M_{bc} [GeV]", nbins,5.2,5.3);
	TH1F * MbcscfHist = new TH1F("MbcscfHist", ";M_{bc} [GeV]", nbins,5.2,5.3);
	TH1F * MbcXsGHist = new TH1F("MbcXsGHist", ";M_{bc} [GeV]", nbins,5.2,5.3);
	
	TH1F * dtHist = new TH1F("dtHist", ";#Delta t [ps]", nbins,-10,10);
	TH1F * dtscfHist = new TH1F("dtscfHist", ";#Delta t [ps]", nbins,-10,10);
	TH1F * dtXsGHist = new TH1F("dtXsGHist", ";#Delta t [ps]", nbins,-10,10);
	
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
	
	B0Signal->Project("dtscfHist", "B0_DeltaT", ( cut + decutdt + mbccutdt + scfB ).c_str());
	B0Signal->Project("dtXsGHist", "B0_DeltaT", ( cut + decutdt + mbccutdt + xsgBkg ).c_str());
	B0Signal->Project("dtHist", "B0_DeltaT", ( cut + decutdt + mbccutdt + trueB ).c_str());
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
	makePretty(dtHist, kBlue, 3004, kWhite);
	
	makePretty(dEscfHist,kBlue+1);
	makePretty(MbcscfHist,kBlue+1);
	makePretty(MVAscfHist,kBlue+1);
	makePretty(dtscfHist,kBlue+1);
	
	makePretty(MVAXsGHist,kRed-4, 1001, kRed-6);
	makePretty(dEXsGHist,kRed-4, 1001, kRed-6);
	makePretty(MbcXsGHist,kRed-4, 1001, kRed-6);
	makePretty(dtXsGHist,kRed-4, 1001, kRed-6);
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
	
	THStack * stackdt = new THStack("stackdt","");
	stackdt->Add(dtXsGHist);
	TH1F * chargeHist = addBkg(stackmbc, stackde, stackmva, stackdt, chargedname, cuts, fccbar, kMagenta-5, 1001, nbins);
	TH1F * lightHist = addBkg(stackmbc, stackde, stackmva, stackdt,  lightname, cuts, fccbar, kCyan-5, 1001, nbins);
	TH1F * ccbarHist = addBkg(stackmbc, stackde, stackmva, stackdt, ccbarname, cuts, fccbar, kCyan-3, 1001, nbins);
	std::cout << "N signal: " << nsignal << std::endl;
	std::cout << "N charged: " << chargeHist->GetEntries() << std::endl;
	std::cout << "N mixed: " << nmixed << std::endl;
	std::cout << "N ccbar: " << lightHist->GetEntries() << std::endl;
	std::cout << "N light: " << ccbarHist->GetEntries() << std::endl;

	stackmbc->Add(MbcHist);
	stackmva->Add(MVAHist);
	stackde->Add(dEHist);
	stackdt->Add(dtHist);
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
	legendMean2->SetHeader("L = 630 fb^{-1}", "C");
	legendMean2->AddEntry(ccbarHist,"c#bar{c}","f");
	legendMean2->AddEntry(lightHist,"u#bar{u}+d#bar{d}+s#bar{s}","f");
	legendMean2->AddEntry(chargeHist,"B^{+}#bar{B}^{-}","f");
	legendMean2->AddEntry(MbcXsGHist,"B^{0}#bar{B}^{0}","f");
	stackmbc->SetTitle(";M_{bc} [GeV];Events");
	stackmbc->GetXaxis()->SetLabelSize(0.04);
	stackmbc->GetYaxis()->SetLabelSize(0.04);
        stackmbc->GetYaxis()->SetTitleOffset(1.3);
	TLatex* txtWatermark = new TLatex(0.2,0.87,"BELLE II SIMULATION") ;
	txtWatermark->SetNDC();
	txtWatermark->SetTextSize(0.05) ;
	txtWatermark->SetTextColor(kRed+3) ;
	txtWatermark->Draw();

	c1->cd(2);

	stackde->Draw("he");
	stackde->SetTitle(";#Delta E [GeV];Events");
	stackde->GetXaxis()->SetLabelSize(0.04);
	stackde->GetYaxis()->SetLabelSize(0.04);
	stackde->SetMaximum( stackde->GetMaximum()*1.15 );
        stackde->GetYaxis()->SetTitleOffset(1.3);
	txtWatermark->Draw();

	c1->cd(3);
	//gPad->SetLogy();
	stackdt->Draw("h");
	stackdt->SetMaximum( stackdt->GetMaximum()*1.25 );
	stackdt->SetTitle(";#Delta T [ps];Events");
	stackdt->GetXaxis()->SetLabelSize(0.04);
	stackdt->GetYaxis()->SetLabelSize(0.04);
	stackdt->GetYaxis()->SetTitleOffset(1.3);
	TLatex* txt = new TLatex(0.2,0.8,"Signal region") ;
	txt->SetNDC();
	txt->SetTextSize(0.05) ;
	txt->SetTextColor(kGray) ;
	txt->Draw();
	txtWatermark->Draw();
	c1->cd(4);
	//gPad->SetLogy();
	stackmva->Draw("he");
	stackmva->SetMaximum( stackmva->GetMaximum()*1.15 );
	stackmva->SetTitle(";MVA score;Events");
	stackmva->GetXaxis()->SetLabelSize(0.04);
	stackmva->GetYaxis()->SetLabelSize(0.04);
	stackmva->GetYaxis()->SetTitleOffset(1.3);
	legendMean2->Draw();
	txtWatermark->Draw();
	//[0]-sqrt((x*x*x+[2]*x+[3]*x+[4]))*[1]
	//[0]*x*sqrt(1-x*x/[1]/[1])*exp(-[2]*sqrt(1-x*x/[1]/[1]))
	//TH2F * dEMbcHist = new TH2F("dEMbcHist", "; #Delta E [GeV];M_{bc} [GeV]", nbins,-0.2,0.2, nbins,5.2,5.3);
	//dEMbcHist->Draw("colz");
}
