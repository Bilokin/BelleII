
TH1F * drawHists(TTree* B0Signal, string name, string cut, string title = "Err", float r1 = 0, float r2 = 1, string mccut =  "(B0_isSignal) &&", TH1F * out = NULL)
{
	int nbins = 50;	
	string observable = name;
	string histname = name;
	if (histname.size() > 10) 
	{
		histname =  name.substr(0,5)+title.substr(0,1);
	}
	TH1F * xPullHist = new TH1F((histname+"Hist").c_str(), (";"+title).c_str(), nbins,r1, r2);
	TH1F * xPullAllHist = new TH1F((histname+"AllHist").c_str(), (";"+title).c_str(), nbins, r1, r2);
	
	B0Signal->Project((histname+"Hist").c_str(), observable.c_str(),(mccut+cut).c_str());
	B0Signal->Project((histname+"AllHist").c_str(), observable.c_str(),(cut).c_str(), "same");
	
	makePretty(xPullHist);
	makePretty(xPullAllHist, kGray+1);
	
	xPullHist->Divide(xPullAllHist);
	xPullHist->Scale(1./xPullHist->GetEntries());
	xPullHist->Draw("he");
	//xPullAllHist->Draw("hesame");
	//xPullAllHist->Scale(1./xPullAllHist->GetEntries());

	out = *&xPullAllHist;
	return xPullHist;
}



void vetos(string signalfile = "signal-veto/mergegrids.root", string bkgfile="ccbar/mergegrids.root")
{
	
	string cut = getCuts();
	
	TCanvas * c1 = new TCanvas("c1", "Dalitz",0,0, 1000,1000);
	c1->Divide(2,2);
	c1->cd(1);
	TChain * chain = new TChain("B0Signal","chain");
	chain->Add(signalfile.c_str());
	chain->Add(bkgfile.c_str());
	TFile * file = TFile::Open(signalfile.c_str());
	TTree* B0Signal = (TTree*)file->Get("B0Signal");
	TH1F * massSignalAllHist = NULL;
	TH1F * massSignalHist = drawHists(chain, "B0_pi0veto_M", "1", "M(#pi)", 0.08,0.2,"B0_pi0veto_mcPDG == 111 &&",massSignalAllHist);
	TF1 * massSignalpdf = new TF1 ("massSignalpdf", "[0]*TMath::Gaus(x,[1],[2])+[3]*TMath::Gaus(x,[4],[5])",0.08,0.2);
	//massSignalHist->Scale(1./massSignalHist->GetEntries());
	//massSignalAllHist->Scale(1./massSignalAllHist->GetEntries());
	//TF1 * massSignalpdf = new TF1 ("massSignalpdf", "[0]+[1]*x+[2]*exp((abs(x)-[3])/[4])",0.08,0.2);
	massSignalpdf->SetParLimits(0,0.0001,1);
	massSignalpdf->SetParLimits(1,0.12,0.15);
	massSignalpdf->SetParLimits(2,0.0001,1);
	massSignalpdf->SetParLimits(3,0.00001,1);
	massSignalpdf->SetParLimits(4,0.12,0.15);
	massSignalpdf->SetParLimits(5,0.001,0.1);
	massSignalHist->Fit("massSignalpdf");
	
	c1->cd(2);
	TH1F * photonSignalHist = drawHists(B0Signal, "B0_pi0veto_gamma1_E", "1", "E(#gamma)", 0.05,0.5,"B0_pi0veto_mcPDG == 111 &&");
	//TF1 * photonSignalpdf = new TF1 ("photonSignalpdf", "[0]*exp(x*[1])",0.05,0.5);
	TF1 * photonSignalpdf = new TF1 ("photonSignalpdf", "[0]/(x-[1])",0.05,0.5);

	photonSignalHist->Fit("photonSignalpdf");
	c1->cd(3);	
	TFile * file2 = TFile::Open(bkgfile.c_str());
	TTree* B0Bkg = (TTree*)file2->Get("B0Signal");
	TH1F * massBkgHist = drawHists(B0Bkg, "B0_pi0veto_M", "1", "M(#pi)", 0.08,0.2,"B0_pi0veto_mcPDG == 111 &&");
	TF1 * massBkgpdf = new TF1 ("massBkgpdf", "[2]*TMath::Gaus(x,[3],[4])+[5]*TMath::Gaus(x,[0],[1]) +[6]*TMath::Gaus(x,[7],[8])",0.08,0.2);
	massBkgpdf->SetParLimits(2,0.01,150000);
	massBkgpdf->SetParLimits(3,0.12,0.15);
	massBkgpdf->SetParLimits(4,0.01,10);
	massBkgpdf->SetParLimits(5,0.1,150000);
	massBkgpdf->SetParLimits(0,0.12,0.15);
	massBkgpdf->SetParLimits(1,0.01,10);
	massBkgpdf->SetParLimits(6,0.01,150000);
	massBkgpdf->SetParLimits(7,0.12,0.15);
	massBkgpdf->SetParLimits(8,0.01,10);
	massBkgHist->Fit("gaus");
	c1->cd(4);
	TH1F * photonBkgHist = drawHists(B0Bkg, "B0_pi0veto_gamma1_E", "1", "E(#gamma)", 0.05,0.5,"B0_pi0veto_mcPDG == 111 &&");
	//TF1 * photonSignalpdf = new TF1 ("photonSignalpdf", "[0]*exp(x*[1])",0.05,0.5);
	TF1 * photonBkgpdf = new TF1 ("photonSignalpdf", "[0]/(x-[1])",0.05,0.5);
}
