#include "fitSettings.C"
void makePretty(TF1* htemp, int color = kBlue)
{
	htemp->SetLineWidth(5);
	htemp->SetLineColor(color);
	if(color != kBlue)
	{
		//htemp->SetLineStyle(2);
	}
	
}


void deltaT(string filename = "test.root")
{
	fitSettings fitResult;
	string cut = getCuts();
	//cut += " && B0_CSMVA > 0.1";
	string trueB = "&& B0_isSignal == 1";
	string bkgB = "&& B0_isSignal == 0";

	TCanvas * c1 = new TCanvas("c1", "Dalitz",0,0, 1500,1000);
	c1->Divide(3,2);
	TFile * file = TFile::Open(filename.c_str());
	TTree* B0Signal = (TTree*)file->Get("B0Signal");
	c1->cd(1);
	TH1F * deltaTBtrueHist = new TH1F("deltaTBtrueHist", ";#Delta t [ps]", 50,-10,10);
	TH1F * deltaTBbkgHist = new TH1F("deltaTBbkgHist", ";#Delta t [ps]", 50,-10,10);
	TH1F * deltaTBHist = new TH1F("deltaTBHist", ";#Delta t [ps]", 50,-10,10);
	
	float neventstrue = B0Signal->Project("deltaTBtrueHist", "(B0_DeltaT)", (cut + trueB).c_str());
	float neventsbkg = B0Signal->Project("deltaTBbkgHist", "(B0_DeltaT)", (cut + bkgB).c_str());
	float nevents = B0Signal->Project("deltaTBHist", "(B0_DeltaT)", (cut).c_str());
	makePretty(deltaTBtrueHist);
	makePretty(deltaTBbkgHist, kGreen +1);
	makePretty(deltaTBHist, kGray+1);
	deltaTBHist->Draw();
	deltaTBtrueHist->Draw("same");
	deltaTBbkgHist->Draw("same");
	//TF1 * deltat = new TF1("deltat","[0]*TMath::Gaus(x,[1],[2])+ [3]*TMath::Gaus(x,0,[4])",-10,10);	
	TF1 * bkgPdf = new TF1("bkgPdf","[0]*TMath::Gaus(x,0,[1])+ [2]*TMath::Gaus(x,0,[3])",-10,10);
	bkgPdf->SetParLimits(1,0.1,10);
	bkgPdf->SetParLimits(3,0.01,2);
	deltaTBbkgHist->Fit("bkgPdf");
	float fbkg = bkgPdf->GetParameter(0)/(bkgPdf->GetParameter(2)+bkgPdf->GetParameter(0));
	fitResult.fbkg[0] = bkgPdf->GetParameter(0)/(bkgPdf->GetParameter(2)+bkgPdf->GetParameter(0));
	fitResult.fsig = neventstrue/nevents;
	fitResult.sigmabkg[0] = bkgPdf->GetParameter(1);
	fitResult.sigmabkg[1] = bkgPdf->GetParameter(3);
	//coreT->Draw("same");
	c1->cd(3);
	
	TH1F * deltaTPullHist = new TH1F("deltaTPullHist", ";#Delta t [ps]", 50,-10,10.);
	TH1F * deltaTPullAllHist = new TH1F("deltaTPullAllHist", ";pull #Delta t ", 50,-10,10.);
	TH2F * ddeltaT2Hist = new TH2F("ddeltaT2Hist", ";#Delta#Delta t [ps]", 50,0,3, 50,0,10 );
	B0Signal->Project("ddeltaT2Hist", "(B0_DeltaT-B0_TruthDeltaT) : B0_DeltaTErr", (cut + trueB ).c_str());
	B0Signal->Project("deltaTPullHist", "(B0_DeltaT-B0_TruthDeltaT)/B0_DeltaTErr", (cut + trueB).c_str());
	B0Signal->Project("deltaTPullAllHist", "((B0_DeltaT-B0_TruthDeltaT))/B0_DeltaTErr", (cut).c_str());
	ddeltaT2Hist->Draw();
	makePretty(deltaTPullHist);
	makePretty(deltaTPullAllHist, kGray+1);
	deltaTPullAllHist->Draw();
	deltaTPullHist->Draw("same");
	deltaTPullHist->Fit("gaus");
	c1->cd(2);
	int nbinsf = 100;
	TH1F * tagBtrueHist = new TH1F("tagBtrueHist", ";FBDT []; Purity", nbinsf,-1,1);
	TH1F * tagBAllHist = new TH1F("tagBAllHist", ";FBDT []", nbinsf,-1,1);
	TH1F * tagBbartrueHist = new TH1F("tagBbartrueHist", ";FBDT []", nbinsf,-1,1);
	tagBtrueHist->Sumw2();
	tagBbartrueHist->Sumw2();
	tagBAllHist->Sumw2();
	B0Signal->Project("tagBtrueHist", "B0_FANN_qrCombined", (cut + trueB + "&& B0_mcPDG == 511").c_str());
	B0Signal->Project("tagBAllHist", "B0_FANN_qrCombined", (cut+trueB).c_str());
	B0Signal->Project("tagBbartrueHist", "B0_FANN_qrCombined", (cut + trueB + "&& B0_mcPDG == -511").c_str());
	tagBtrueHist->Divide(tagBAllHist);
	tagBbartrueHist->Divide(tagBAllHist);
	makePretty(tagBtrueHist);
	makePretty(tagBAllHist, kGray+1);
	makePretty(tagBbartrueHist,kRed);
	//tagBAllHist->Draw();
	tagBtrueHist->Draw("he");
	tagBbartrueHist->Draw("samehe");
	float qlow = 0.5; float qhigh = 1.;
	float puritybar = tagBbartrueHist->Integral(3,4)/(tagBbartrueHist->Integral(3,4) + tagBtrueHist->Integral(3,4));
	float purity =       tagBtrueHist->Integral( 1 ,2)/(tagBbartrueHist->Integral( 1, 2) + tagBtrueHist->Integral( 1, 2));
	//float shift = (tagBtrueHist->Integral(-qhigh,qhigh)-tagBbartrueHist->Integral(-qhigh,qhigh))/(tagBbartrueHist->Integral(-qhigh,qhigh) + tagBtrueHist->Integral(-qhigh,qhigh));
	float shift = purity - puritybar;
	c1->cd(4);
	
	TH1F * deltaTResHist = new TH1F("deltaTResHist", ";#Delta t [ps]", 50,-10,10.);
	TH1F * deltaTResAllHist = new TH1F("deltaTResAllHist", ";Residual #Delta t ", 50,-10,10.);
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
	//TF1 * deltatRes = new TF1("deltatRes","[0]*TMath::Gaus(x,[1],[2]) + [3]*TMath::Gaus(x,[4],[5]) + [6] * TMath::Gaus(x,[7],[8])",-10,10);
	TF1 * deltatRes = new TF1("deltatRes","[0]*TMath::Gaus(x,0,[1]) + [2]*TMath::Gaus(x,0,[3]) + [4] * TMath::Gaus(x,0,[5])",-10,10);
	makePretty(deltatRes, kGray+1);
	//deltatRes->SetParLimits(0, nevents/1000, nevents);
	//deltatRes->SetParLimits(1, -0.1, 0.1);
	deltatRes->SetParLimits(1, 0.1, 2);
	//deltatRes->SetParLimits(3, nevents/1000, nevents/20);
	//deltatRes->SetParLimits(4, -1, 1);
	deltatRes->SetParLimits(3, 0.1, 5);
	//deltatRes->SetParLimits(7, -0.2, 0.2);
	deltatRes->SetParLimits(5, 0.01, 5);
	deltaTResHist->Fit("deltatRes");
	float fres1 = deltatRes->GetParameter(0)/(deltatRes->GetParameter(2)+deltatRes->GetParameter(0) + deltatRes->GetParameter(4));
	float fres2 = deltatRes->GetParameter(2)/(deltatRes->GetParameter(2)+deltatRes->GetParameter(0) + deltatRes->GetParameter(4));
	float fres3 = deltatRes->GetParameter(4)/(deltatRes->GetParameter(2)+deltatRes->GetParameter(0) + deltatRes->GetParameter(4));
	TF1 * core = new TF1("core", "gaus", -10,10);
	core->SetParameters(deltatRes->GetParameter(0), 0,deltatRes->GetParameter(1));
	makePretty(core);
	TF1 * bkg1 = new TF1("bkg1", "gaus", -10,10);
	bkg1->SetParameters(deltatRes->GetParameter(3), deltatRes->GetParameter(4),deltatRes->GetParameter(5));
	makePretty(bkg1, kRed);
	TF1 * bkg2 = new TF1("bkg2", "gaus", -10,10);
	bkg2->SetParameters(deltatRes->GetParameter(6), deltatRes->GetParameter(7),deltatRes->GetParameter(8));
	makePretty(bkg2, kGreen);
	cout << "P: " << deltatRes->GetParameter(0) << endl;
	fitResult.fres[0] = deltatRes->GetParameter(0)/(deltatRes->GetParameter(2)+deltatRes->GetParameter(0) + deltatRes->GetParameter(4));
	fitResult.fres[1] = deltatRes->GetParameter(2)/(deltatRes->GetParameter(2)+deltatRes->GetParameter(0) + deltatRes->GetParameter(4));
	fitResult.fres[2] = deltatRes->GetParameter(4)/(deltatRes->GetParameter(2)+deltatRes->GetParameter(0) + deltatRes->GetParameter(4));
	fitResult.sigmares[0] = deltatRes->GetParameter(1);
	fitResult.sigmares[1] = deltatRes->GetParameter(3);
	fitResult.sigmares[2] = deltatRes->GetParameter(5);
	//core->Draw("same");
	//bkg1->Draw("same");
	//bkg2->Draw("same");
	c1->cd(5);

	TH1F * deltaTBPHist = new TH1F("deltaTBPHist", ";#Delta t [ps]", 50,-10,10);
	TH1F * deltaTBbarPHist = new TH1F("deltaTBbarPHist", ";#Delta t [ps]", 50,-10,10);

        B0Signal->Project("deltaTBPHist", "B0_DeltaT", (cut + bkgB + " && B0_FANN_qrCombined > 0").c_str());
        B0Signal->Project("deltaTBbarPHist", "B0_DeltaT", (cut + bkgB + " && B0_FANN_qrCombined < 0").c_str());
        //B0Signal->Project("deltaTBPHist", "B0_DeltaT", (cut + "&& B0_mcPDG == 511").c_str());
        //B0Signal->Project("deltaTBbarPHist", "B0_DeltaT", (cut + "&& B0_mcPDG == -511").c_str());
	makePretty(deltaTBPHist);
	makePretty(deltaTBbarPHist,kRed);
	deltaTBPHist->Draw();
	deltaTBbarPHist->Draw("same");
	
	c1->cd(6);
	TH1F * deltaTErr = new TH1F("deltaTErr", ";#Delta t uncertainty [ps]", 50,0,5);
	TH1F * deltaTtrueErr = new TH1F("deltaTtrueErr", ";#Delta t [ps]", 50,0,5);

        B0Signal->Project("deltaTtrueErr", "B0_DeltaTErr", (cut + trueB).c_str());
        B0Signal->Project("deltaTErr", "B0_DeltaTErr", (cut ).c_str());
	makePretty(deltaTtrueErr);
	makePretty(deltaTErr,kGray+1);

	deltaTErr->Draw();
	deltaTtrueErr->Draw("same");
	
	TCanvas * c2 = new TCanvas("c2", "Dalitz", 0, 0, 500, 500);
	c2->cd();
	int nbins = 50;
	string dalitzcut = "";
	TH1F * BHist = new TH1F("BHist", ";#Delta t [ps]", nbins,-10,10);
	TH1F * BbarHist = new TH1F("BbarHist", ";#Delta t [ps]", nbins,-10,10);
	TH1F * genBHist = new TH1F("genBHist", ";#Delta t [ps]", nbins,-10,10);
	TH1F * genBbarHist = new TH1F("genBbarHist", ";#Delta t [ps]", nbins,-10,10);

        B0Signal->Project("genBHist", "B0_TruthDeltaT", (cut + dalitzcut + "&& B0_mcPDG == 511").c_str());
        B0Signal->Project("genBbarHist", "B0_TruthDeltaT", (cut + dalitzcut +"&& B0_mcPDG == -511").c_str());
        B0Signal->Project("BHist", "B0_DeltaT", (cut +dalitzcut+ "&& B0_FANN_qrCombined > 0.6").c_str());
        B0Signal->Project("BbarHist", "B0_DeltaT", (cut + dalitzcut+"&& B0_FANN_qrCombined < -0.6").c_str());
	makePretty(BHist);
	makePretty(BbarHist,kRed);
	//genBHist->Scale(BHist->GetMaximum()/genBHist->GetMaximum());
	//genBbarHist->Scale(BHist->GetMaximum()/genBbarHist->GetMaximum());
	genBHist->SetLineWidth(3);
	genBHist->SetLineColor(kGray);
	genBbarHist->SetLineWidth(3);
	genBbarHist->SetLineColor(kGray+1);
	genBHist->Scale(1./genBHist->GetEntries());
	genBbarHist->Scale(1./genBbarHist->Integral());
	genBHist->Draw("h");
	genBbarHist->Draw("esame");
        TF1 * myBdeltaT = new TF1("myBdeltaT",      "0.81*exp(-abs(x)/1.5)/(4*1.5)*( 1 + ([0]*cos(0.51*x)+[1]*sin(0.51*x)) )",-10,10);
	genBHist->Fit("myBdeltaT");
		std::cout << "-------------------------------------------------"  << std::endl;
	cout <<  "nEvents: " << nevents << " signal fraction: " << fitResult.fsig << endl;
	std::cout << "Purity: " << puritybar << " shift: " << shift << std::endl;
	std::cout << "fres1: " << fitResult.fres[0] << " fres2: " << fitResult.fres[1] << " fres3: " << fitResult.fres[2] << " | s1: " << fitResult.sigmares[0]  << " s2: " << fitResult.sigmares[1] << " s3: " << fitResult.sigmares[2]  << std::endl;
	std::cout << "fbkg: " <<  fitResult.fbkg[0] << " s1: " << fitResult.sigmabkg[0]  << " s2: " << fitResult.sigmabkg[1]  << std::endl;
	//BHist->Draw("hesame");
	//BbarHist->Draw("hesame");
}
