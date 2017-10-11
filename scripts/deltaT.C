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
	htemp->SetMinimum(0);
}
void makePretty(TF1* htemp, int color = kBlue)
{
	htemp->SetLineWidth(5);
	htemp->SetLineColor(color);
	if(color != kBlue)
	{
		//htemp->SetLineStyle(2);
	}
	
}

float myExp(float x, float slopeM, float slopeP) {return (x>0)?  std::exp(slopeM*x): std::exp(-slopeP*x); }

void deltaT(string filename = "test.root")
{
	string cut = "B0_mbc > 5.27 && B0_deltae > -0.2 && B0_deltae < 0.1"; // BEST
	string trueB = "&& abs(B0_mcPDG) == 511";

	TCanvas * c1 = new TCanvas("c1", "Dalitz",0,0, 1500,1000);
	c1->Divide(3,2);
	TFile * file = TFile::Open(filename.c_str());
	TTree* B0Signal = (TTree*)file->Get("B0Signal");
	c1->cd(1);
	TH1F * deltaTBtrueHist = new TH1F("deltaTBtrueHist", ";#Delta t [ps]", 50,0,10);
	TH1F * deltaTBHist = new TH1F("deltaTBHist", ";#Delta t [ps]", 50,0,10);
	
	B0Signal->Project("deltaTBtrueHist", "abs(B0_DeltaT)", (cut + trueB).c_str());
	float nevents = B0Signal->Project("deltaTBHist", "abs(B0_DeltaT)", (cut).c_str());
	makePretty(deltaTBtrueHist);
	makePretty(deltaTBHist, kGray+1);
	deltaTBHist->Draw();
	deltaTBtrueHist->Draw("same");
	//TF1 * deltat = new TF1("deltat","[0]*TMath::Gaus(x,[1],[2])+ [3]*TMath::Gaus(x,0,[4])",-10,10);	
	TF1 * deltat = new TF1("deltat","expo",.5,10);	
	//deltat->SetParLimits(0,1,1000);
	//deltat->SetParLimits(1,-1,1);
	//deltat->SetParLimits(2,0.01,10);
	//deltat->SetParLimits(3,1,100);
	//deltat->SetParLimits(4,0.01,10);
	deltaTBtrueHist->Fit("deltat","R");
	cout <<  "nEvents: " << nevents << " Tau: " << 1./deltat->GetParameter(1) << endl;
	c1->cd(2);
	
	TH1F * tagBtrueHist = new TH1F("tagBtrueHist", ";FBDT []; Purity", 5,-1,1);
	TH1F * tagBAllHist = new TH1F("tagBAllHist", ";FBDT []", 5,-1,1);
	TH1F * tagBbartrueHist = new TH1F("tagBbartrueHist", ";FBDT []", 5,-1,1);
	tagBtrueHist->Sumw2();
	tagBbartrueHist->Sumw2();
	tagBAllHist->Sumw2();
	B0Signal->Project("tagBtrueHist", "B0_FANN_qrCombined", (cut + "&& B0_mcPDG == 511").c_str());
	B0Signal->Project("tagBAllHist", "B0_FANN_qrCombined", (cut).c_str());
	B0Signal->Project("tagBbartrueHist", "B0_FANN_qrCombined", (cut + "&& B0_mcPDG == -511").c_str());
	tagBtrueHist->Divide(tagBAllHist);
	tagBbartrueHist->Divide(tagBAllHist);
	makePretty(tagBtrueHist);
	makePretty(tagBAllHist, kGray+1);
	makePretty(tagBbartrueHist,kRed);
	//tagBAllHist->Draw();
	tagBtrueHist->Draw("he");
	tagBbartrueHist->Draw("samehe");
	c1->cd(4);
	
	TH1F * deltaTResHist = new TH1F("deltaTResHist", ";#Delta t [ps]", 50,-10,10.);
	TH1F * deltaTResAllHist = new TH1F("deltaTResAllHist", ";Resolution #Delta t ", 50,-10,10.);
	//TH2F * deltaT2Hist = new TH2F("deltaT2Hist", ";#Delta t [ps]", 50,-10,10, 50,-10,10 );
	TH2F * deltaT2Hist = new TH2F("deltaT2Hist", ";#Delta t [ps]", 50,0,0.1, 50,0,3 );
	
	//B0Signal->Project("deltaT2Hist", "B0_DeltaT:B0_TruthDeltaT", (cut + trueB).c_str());
	B0Signal->Project("deltaT2Hist", "abs((B0_DeltaT-B0_TruthDeltaT)/B0_TruthDeltaT) : abs(B0_TagVz-B0_Z)", (cut + trueB).c_str());
	B0Signal->Project("deltaTResHist", "(B0_DeltaT-B0_TruthDeltaT)", (cut + trueB).c_str());
	B0Signal->Project("deltaTResAllHist", "((B0_DeltaT-B0_TruthDeltaT))", (cut).c_str());
	makePretty(deltaTResHist);
	makePretty(deltaTResAllHist, kGray+1);
	deltaT2Hist->Draw("colz");
	deltaTResAllHist->Draw();
	deltaTResHist->Draw("same");
	//TF1 * deltatRes = new TF1("deltatRes","gaus+gaus(3)",-10,10);

	//TF1 * deltatRes = new TF1("deltatRes","[0]*TMath::Gaus(x,[1],[2]) + (1.-[0])*([3]*TMath::Gaus(x,[4],[5]) + (1.-[3]) * TMath::Gaus(x,[6],[7]) )",-10,10);
	TF1 * deltatRes = new TF1("deltatRes","[0]*TMath::Gaus(x,[1],[2]) + [3]*TMath::Gaus(x,[4],[5]) + [6] * TMath::Gaus(x,[7],[8])",-10,10);
	//deltatRes->SetParLimits(0, nevents/1000, nevents);
	deltatRes->SetParLimits(1, -0.1, 0.1);
	deltatRes->SetParLimits(2, 0.1, 2);
	//deltatRes->SetParLimits(3, nevents/1000, nevents/20);
	deltatRes->SetParLimits(4, -1, 1);
	deltatRes->SetParLimits(5, 0.1, 5);
	deltatRes->SetParLimits(7, -0.2, 0.2);
	deltatRes->SetParLimits(8, 0.01, 5);
	deltaTResAllHist->Fit("deltatRes");
	TF1 * core = new TF1("core", "gaus", -10,10);
	core->SetParameters(deltatRes->GetParameter(0), deltatRes->GetParameter(1),deltatRes->GetParameter(2));
	makePretty(core);
	TF1 * bkg1 = new TF1("bkg1", "gaus", -10,10);
	bkg1->SetParameters(deltatRes->GetParameter(3), deltatRes->GetParameter(4),deltatRes->GetParameter(5));
	makePretty(bkg1, kRed);
	TF1 * bkg2 = new TF1("bkg2", "gaus", -10,10);
	bkg2->SetParameters(deltatRes->GetParameter(6), deltatRes->GetParameter(7),deltatRes->GetParameter(8));
	makePretty(bkg2, kGreen);
	cout << "P: " << deltatRes->GetParameter(0) << endl;
	core->Draw("same");
	bkg1->Draw("same");
	bkg2->Draw("same");
	c1->cd(5);

	TH1F * deltaTBPHist = new TH1F("deltaTBPHist", ";#Delta t [ps]", 50,-10,10);
	TH1F * deltaTBbarPHist = new TH1F("deltaTBbarPHist", ";#Delta t [ps]", 50,-10,10);

        B0Signal->Project("deltaTBPHist", "B0_DeltaT", (cut + "&& B0_mcPDG == 511").c_str());
        B0Signal->Project("deltaTBbarPHist", "B0_DeltaT", (cut + "&& B0_mcPDG == -511").c_str());
	makePretty(deltaTBPHist);
	makePretty(deltaTBbarPHist,kRed);
	deltaTBPHist->Draw();
	deltaTBbarPHist->Draw("same");

}
