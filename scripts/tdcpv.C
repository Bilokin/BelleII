#include "convolution2.C"

void addBranches(TTree* T)
{
	float dt = 0;
	int q = 1;
	float fann = 0;
	TBranch *bdt = T->Branch("dt",&dt,"dt/F");
	TBranch *bq = T->Branch("q",&q,"q/I");
	T->SetBranchAddress("B0_FANN_qrCombined",&fann);
	T->SetBranchAddress("B0_DeltaT",&dt);
	Long64_t nentries = T->GetEntries();
	float A = 0.5;
	float S = 0.5;
	float dM = 0.51;
	float tau = 1.58;
	for (Long64_t i=0; i < nentries; i++) 
	{
		T->GetEntry(i);
		q = TMath::Sign(1,fann);
		//dt = exp(-abs(dt)/tau)/4/tau*(1+q*(A * cos(dM*dt) + S * sin(dM*dt)));
		//std::cout << "dt: " << dt << " q: " << q << std::endl;
		bdt->Fill();
		bq->Fill();
	}
	T->Write();
	//T->Print();
}

void tdcpv(string filename = "test.root")
{
	string cut = getCuts();
	string trueB = "&& B0_isSignal == 1";
	string cutB = "&& B0_FANN_qrCombined > 0.5";
	string cutBbar = "&& B0_FANN_qrCombined < -0.5";
	TCanvas * c1 = new TCanvas("c1", "Dalitz",0,0, 500,500);
	//c1->Divide(3,2);
	TFile * file = TFile::Open(filename.c_str());
	TTree* B0Signal = (TTree*)file->Get("B0Signal");
	TFile * f2 = new TFile("tmp-branch.root","recreate");
	TTree* B0Signal2 = B0Signal->CopyTree(cut.c_str());
	//B0Signal2->Scan("B0_DeltaTErr");
	addBranches(B0Signal2);
	f2->Write();
	file->cd();
	//c1->cd(1);
	int nbins = 100;
	TH1F * deltaTBHist = new TH1F("deltaTBHist", ";#Delta t [ps]", nbins,-10,10);
	TH1F * deltaTBbarHist = new TH1F("deltaTBbarHist", ";#Delta t [ps]", nbins,-10,10);
	
	float neventsb = B0Signal->Project("deltaTBHist", "(B0_DeltaT)", (cut+cutB).c_str());
	float neventsbbar = B0Signal->Project("deltaTBbarHist", "(B0_DeltaT)", (cut+cutBbar).c_str());
	makePretty(deltaTBHist);
	makePretty(deltaTBbarHist,kRed);
	deltaTBHist->Draw("he");
	deltaTBbarHist->Draw("hesame");
	std::cout << "B: " << neventsb << " Bbar: " << neventsbbar << std::endl;
	f2->cd();
	convolution2(B0Signal2, 0.8);
	//convolution(createRooHist(deltaTBHist), createRooHist(deltaTBbarHist));
}
