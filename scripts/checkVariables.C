void checkVariables(string signalname = "test.root", string continuumname = "", bool signalEnhanced = false)
{
	TCanvas * c1 = new TCanvas("c1", "Dalitz",0,0, 1500,500);
	c1->Divide(3,1);
	c1->cd(1);
	int nbins = 50;
	TH1F * dEqqHist = NULL;
	TH1F * MbcqqHist = NULL;
	TH1F * MVAqqHist = NULL;
	
	string cut = getBasicCuts();
	//cut += " && B0_gamma_MC_MOTHER_ID != 111 && B0_gamma_MC_MOTHER_ID != 221";
	string trueB = "&& B0_isSignal == 1";
	//string scfB = "&& B0_isSignal == 0 && B0_isContinuumEvent == 0 ";
	//string xsgBkg = "&& B0_isSignal == 0 && B0_isContinuumEvent == 0 && 0";
	string scfB = "&& B0_isSignal == 0 && B0_isContinuumEvent == 0 && (B0_mcErrors == 306 || B0_mcErrors == 512)";
	string xsgBkg = "&& B0_isSignal == 0 && B0_isContinuumEvent == 0 && B0_mcErrors != 306 && B0_mcErrors != 512";
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
	  /////////////////////////////////////////
	 //	    SIGNAL+SCF+XsGamma	        //
	/////////////////////////////////////////	
	TFile * fileSig = TFile::Open(signalname.c_str());
	fileSig->cd();
	TTree* B0Signal = (TTree*)fileSig->Get("B0Signal");
	TH1F * dEHist = new TH1F("dEHist", ";#Delta E [GeV]", nbins,-0.2,0.2);
	TH1F * dEscfHist = new TH1F("dEscfHist", ";#Delta E [GeV]", nbins,-0.2,0.2);
	TH1F * dEXsGHist = new TH1F("dEXsGHist", ";#Delta E [GeV]", nbins,-0.2,0.2);
	
	TH1F * MbcHist = new TH1F("MbcHist", ";M_{bc} [GeV]", nbins,5.2,5.3);
	TH1F * MbcscfHist = new TH1F("MbcscfHist", ";M_{bc} [GeV]", nbins,5.2,5.3);
	TH1F * MbcXsGHist = new TH1F("MbcXsGHist", ";M_{bc} [GeV]", nbins,5.2,5.3);
	
	TH1F * MVAHist = new TH1F("MVAHist", ";MVA", nbins,0,1);
	TH1F * MVAscfHist = new TH1F("MVAscfHist", ";MVA", nbins,0,1);
	TH1F * MVAXsGHist = new TH1F("MVAXsGHist", ";MVA", nbins,0,1);
	B0Signal->Project("dEMbcHist", "B0_mbc : B0_deltae", ( cut ).c_str());
	B0Signal->Project("dEHist", "B0_deltae", ( cut + mvacut + mbccut + trueB ).c_str());
	B0Signal->Project("MbcHist", "B0_mbc", ( cut + decut + mvacut + trueB ).c_str());
	B0Signal->Project("dEscfHist", "B0_deltae", ( cut + mbccut + mvacut + scfB ).c_str());
	B0Signal->Project("dEXsGHist", "B0_deltae", ( cut + mbccut + mvacut + xsgBkg ).c_str());
	B0Signal->Project("MbcscfHist", "B0_mbc", ( cut + decut + mvacut + scfB ).c_str());
	B0Signal->Project("MbcXsGHist", "B0_mbc", ( cut + decut + mvacut + xsgBkg ).c_str());
	B0Signal->Project("MVAscfHist", "B0_CSMVA", ( cut + decut + mbccut + scfB ).c_str());
	B0Signal->Project("MVAXsGHist", "B0_CSMVA", ( cut + decut + mbccut + xsgBkg ).c_str());
	B0Signal->Project("MVAHist", "B0_CSMVA", ( cut + decut + mbccut + trueB ).c_str());
	float fsignal = 0.000577368*4;
	dEHist->Scale(fsignal);
	dEscfHist->Scale(fsignal);
	dEXsGHist->Scale(fsignal);
	MbcHist->Scale(fsignal);
	MbcscfHist->Scale(fsignal);
	MbcXsGHist->Scale(fsignal);
	MVAHist->Scale(fsignal);
	MVAscfHist->Scale(fsignal);
	MVAXsGHist->Scale(fsignal);
	
	makePretty(dEHist, kBlue, 3001, kWhite);
	makePretty(MbcHist, kBlue, 3001, kWhite);
	makePretty(MVAHist, kBlue, 3001, kWhite);
	
	makePretty(dEscfHist,kBlue+1);
	makePretty(MbcscfHist,kBlue+1);
	makePretty(MVAscfHist,kBlue+1);
	
	makePretty(MVAXsGHist,kGreen+1, 3002, kGreen);
	makePretty(dEXsGHist,kGreen+1, 3002, kGreen);
	makePretty(MbcXsGHist,kGreen+1, 3002, kGreen);
	  /////////////////////////////////////////
	 //		CONTINUUM		//
	/////////////////////////////////////////	
	if (continuumname!="") 
	{
		TFile * fileBkg = TFile::Open(continuumname.c_str());
		TTree* B0Continuum = (TTree*)fileBkg->Get("B0Signal");
		dEqqHist = new TH1F("dEqqHist", ";#Delta E [GeV]", nbins,-0.2,0.2);
		MbcqqHist = new TH1F("MbcqqHist", ";M_{bc} [GeV]", nbins,5.2,5.3);
		MVAqqHist = new TH1F("MVAqqHist", ";MVA", nbins,0,1);
		B0Continuum->Project("dEqqHist", "B0_deltae", ( cut + mvacut + mbccut + continuum ).c_str());
		B0Continuum->Project("MbcqqHist", "B0_mbc", ( cut + mvacut + decut + continuum ).c_str());
		B0Continuum->Project("MVAqqHist", "B0_CSMVA", ( cut + mbccut + decut + continuum ).c_str());
		makePretty(dEqqHist, kGray+1,3001);
		makePretty(MbcqqHist, kGray+1,3001);
		makePretty(MVAqqHist, kGray+1,3001);
	}

	


	THStack * stackmbc = new THStack("stackmbc","stackde");
	stackmbc->Add(MbcXsGHist);
	stackmbc->Add(MbcqqHist);
	stackmbc->Add(MbcscfHist);
	stackmbc->Add(MbcHist);
	stackmbc->Draw("he");
	stackmbc->SetMaximum( stackmbc->GetMaximum()*1.15 );
	TLegend *legendMean2 = new TLegend(0.15,0.65,0.5,0.85,NULL,"brNDC");
	legendMean2->SetFillStyle(0);
	legendMean2->SetBorderSize(0);
	legendMean2->AddEntry(MbcHist,"Signal","f");
	legendMean2->AddEntry(MbcscfHist,"SCF","f");
	legendMean2->AddEntry(MbcqqHist,"Continuum","f");
	legendMean2->AddEntry(MbcXsGHist,"X_{s} #gamma bkg","f");
	legendMean2->Draw();
	stackmbc->SetTitle(";M_{bc} [GeV]");
	stackmbc->GetXaxis()->SetLabelSize(0.04);
	stackmbc->GetYaxis()->SetLabelSize(0.04);
	c1->cd(2);

	THStack * stackde = new THStack("stackde","");
	stackde->Add(dEXsGHist);
	stackde->Add(dEqqHist);
	stackde->Add(dEscfHist);
	stackde->Add(dEHist);
	stackde->Draw("he");
	stackde->SetTitle(";#Delta E [GeV]");
	stackde->GetXaxis()->SetLabelSize(0.04);
	stackde->GetYaxis()->SetLabelSize(0.04);
	stackde->SetMaximum( stackde->GetMaximum()*1.15 );
	c1->cd(3);

	THStack * stackmva = new THStack("stackmva","");
	stackmva->Add(MVAXsGHist);
	stackmva->Add(MVAqqHist);
	stackmva->Add(MVAscfHist);
	stackmva->Add(MVAHist);
	stackmva->Draw("he");
	stackmva->SetTitle(";MVA score");
	stackmva->GetXaxis()->SetLabelSize(0.04);
	stackmva->GetYaxis()->SetLabelSize(0.04);
	//[0]-sqrt((x*x*x+[2]*x+[3]*x+[4]))*[1]
	//[0]*x*sqrt(1-x*x/[1]/[1])*exp(-[2]*sqrt(1-x*x/[1]/[1]))
	//TH2F * dEMbcHist = new TH2F("dEMbcHist", "; #Delta E [GeV];M_{bc} [GeV]", nbins,-0.2,0.2, nbins,5.2,5.3);
	//dEMbcHist->Draw("colz");
	
}
