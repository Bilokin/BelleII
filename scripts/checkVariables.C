void checkVariables(string signalname = "test.root", string continuumname = "")
{
	TCanvas * c1 = new TCanvas("c1", "Dalitz",0,0, 1000,1000);
	c1->Divide(2,2);
	c1->cd(1);
	int nbins = 50;
	TH1F * dEqqHist = NULL;
	TH1F * MbcqqHist = NULL;
	
	string cut = getBasicCuts();
	string trueB = "&& B0_isSignal == 1";
	string scfB = "&& B0_isSignal == 0 && B0_isContinuumEvent == 0";
	string continuum = "&& B0_isContinuumEvent == 1";
	
	string decut = "&& B0_deltae > -0.15 && B0_deltae < 0.1";
	string mbccut = "&& B0_mbc > 5.27";
	  /////////////////////////////////////////
	 //		SIGNAL+SCF		//
	/////////////////////////////////////////	
	TFile * fileSig = TFile::Open(signalname.c_str());
	fileSig->cd();
	TTree* B0Signal = (TTree*)fileSig->Get("B0Signal");
	TH1F * dEHist = new TH1F("dEHist", ";#Delta E [GeV]", nbins,-0.2,0.2);
	TH1F * MbcHist = new TH1F("MbcHist", ";M_{bc} [GeV]", nbins,5.2,5.3);
	TH1F * dEscfHist = new TH1F("dEscfHist", ";#Delta E [GeV]", nbins,-0.2,0.2);
	TH1F * MbcscfHist = new TH1F("MbcscfHist", ";M_{bc} [GeV]", nbins,5.2,5.3);
	B0Signal->Project("dEMbcHist", "B0_mbc : B0_deltae", ( cut ).c_str());
	B0Signal->Project("dEHist", "B0_deltae", ( cut + mbccut + trueB ).c_str());
	B0Signal->Project("MbcHist", "B0_mbc", ( cut + decut + trueB ).c_str());
	B0Signal->Project("dEscfHist", "B0_deltae", ( cut + mbccut + scfB ).c_str());
	B0Signal->Project("MbcscfHist", "B0_mbc", ( cut + decut + scfB ).c_str());
	float fsignal = 1./50;
	dEHist->Scale(fsignal);
	MbcHist->Scale(fsignal);
	dEscfHist->Scale(fsignal);
	MbcscfHist->Scale(fsignal);
	
	makePretty(dEHist, kBlue, 3001, kWhite);
	makePretty(MbcHist, kBlue, 3001, kWhite);
	
	makePretty(dEscfHist,kBlue+1);
	makePretty(MbcscfHist,kBlue+1);
	  /////////////////////////////////////////
	 //		CONTINUUM		//
	/////////////////////////////////////////	
	if (continuumname!="") 
	{
		TFile * fileBkg = TFile::Open(continuumname.c_str());
		TTree* B0Continuum = (TTree*)fileBkg->Get("B0Signal");
		dEqqHist = new TH1F("dEqqHist", ";#Delta E [GeV]", nbins,-0.2,0.2);
		MbcqqHist = new TH1F("MbcqqHist", ";M_{bc} [GeV]", nbins,5.2,5.3);
		B0Continuum->Project("dEqqHist", "B0_deltae", ( cut + mbccut + continuum ).c_str());
		B0Continuum->Project("MbcqqHist", "B0_mbc", ( cut + decut + continuum ).c_str());
		makePretty(dEqqHist, kGray+1,3001);
		makePretty(MbcqqHist, kGray+1,3001);
	}

	


	THStack * stackmbc = new THStack("stackmbc","stackde");
	stackmbc->Add(MbcqqHist);
	stackmbc->Add(MbcscfHist);
	stackmbc->Add(MbcHist);
	stackmbc->Draw("he");
	stackmbc->SetMaximum( stackmbc->GetMaximum()*1.15 );
	TLegend *legendMean2 = new TLegend(0.15,0.65,0.5,0.85,NULL,"brNDC");
	legendMean2->SetFillColor(0);
	legendMean2->SetBorderSize(0);
	legendMean2->AddEntry(MbcHist,"Signal","f");
	legendMean2->AddEntry(MbcscfHist,"SCF","f");
	legendMean2->AddEntry(MbcqqHist,"Continuum","f");
	legendMean2->Draw();
	stackmbc->SetTitle(";M_{bc} [GeV]");
	stackmbc->GetXaxis()->SetLabelSize(0.04);
	stackmbc->GetYaxis()->SetLabelSize(0.04);
	c1->cd(2);

	THStack * stackde = new THStack("stackde","");
	stackde->Add(dEqqHist);
	stackde->Add(dEscfHist);
	stackde->Add(dEHist);
	stackde->Draw("he");
	stackde->SetTitle(";#Delta E [GeV]");
	stackde->GetXaxis()->SetLabelSize(0.04);
	stackde->GetYaxis()->SetLabelSize(0.04);
	//TH2F * dEMbcHist = new TH2F("dEMbcHist", "; #Delta E [GeV];M_{bc} [GeV]", nbins,-0.2,0.2, nbins,5.2,5.3);
	//dEMbcHist->Draw("colz");
	
}
