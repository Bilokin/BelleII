void drawHists(TTree* B0Signal, string name, string cut, string err = "Err", float range = 5, string mccut =  "(B0_isSignal) &&")
{
	string truth = "Truth";
	string b = "B0_";
	int nbins = 150;	
	string observable = "("+b+name + "-" + b+truth+name +")/"+b+err+name;
	if (err != "Err") 
	{
		observable = "("+b+name + "-" + b+truth+name +")/"+b+err;
	}
	TH1F * xPullHist = new TH1F((name+"Hist").c_str(), "; [cm]", nbins,-range, range);
	TH1F * xPullAllHist = new TH1F((name+"AllHist").c_str(), (";"+observable).c_str(), nbins,-range, range);
	
	B0Signal->Project((name+"Hist").c_str(), observable.c_str(),(mccut+cut).c_str());
	B0Signal->Project((name+"AllHist").c_str(), observable.c_str(),(cut).c_str(), "same");
	
	makePretty(xPullHist);
	makePretty(xPullAllHist, kGray+1);
	
	xPullAllHist->Draw("he");
	xPullHist->Draw("hesame");
}

void vtxPulls(string filename = "test.root")
{
	string mccut= "(B0_isSignal) &&";
	TCanvas * c1 = new TCanvas("c1", "Dalitz",0,0, 1500,1000);
	c1->Divide(3,2);
	c1->cd(1);
	TFile * file = TFile::Open(filename.c_str());
	TTree* B0Signal = (TTree*)file->Get("B0Signal");
	string cut = getBasicCuts();
	
	drawHists(B0Signal, "X", cut);
	c1->cd(2);
	drawHists(B0Signal, "Y", cut);
	c1->cd(3);
	drawHists(B0Signal, "Z", cut);
	c1->cd(4);
	drawHists(B0Signal, "TagVx", cut, "TagVex");
	c1->cd(5);
	drawHists(B0Signal, "TagVy", cut, "TagVey");
	c1->cd(6);
	drawHists(B0Signal, "TagVz", cut, "TagVez");
}
