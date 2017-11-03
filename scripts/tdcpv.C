void tdcpv(string filename = "test.root")
{
	string cut = "B0_mbc > 5.27 && B0_deltae > -0.2 && B0_deltae < 0.1"; // BEST
	cut += " && abs(B0_K_10_K_S0_M - 0.497) < 0.01 ";
	cut += " && B0_DeltaTErr < 2.5 ";
	cut += " && B0_m12 > 0.6 && B0_m12 < 0.9";
	cut += " && (B0_m23 > 0.95 ||  B0_m23 < 0.85) && (B0_m13 > 0.95 ||  B0_m13 < 0.85)";
	cut += " && B0_K_10_M < 1.8 ";
	cut += " && B0_K_10_K_S0_Rho > 0.15";
	//cut += " && abs(B0_FANN_qrCombined) > 0.1 ";
	string trueB = "&& B0_isSignal == 1";
	string cutB = "&& B0_FANN_qrCombined > 0.5";
	string cutBbar = "&& B0_FANN_qrCombined < -0.5";
	TCanvas * c1 = new TCanvas("c1", "Dalitz",0,0, 500,500);
	//c1->Divide(3,2);
	TFile * file = TFile::Open(filename.c_str());
	TTree* B0Signal = (TTree*)file->Get("B0Signal");
	//c1->cd(1);
	TH1F * deltaTBHist = new TH1F("deltaTBHist", ";#Delta t [ps]", 50,-10,10);
	TH1F * deltaTBbarHist = new TH1F("deltaTBbarHist", ";#Delta t [ps]", 50,-10,10);
	
	float neventsb = B0Signal->Project("deltaTBHist", "(B0_DeltaT)", (cut+cutB).c_str());
	float neventsbbar = B0Signal->Project("deltaTBbarHist", "(B0_DeltaT)", (cut+cutBbar).c_str());
	makePretty(deltaTBHist);
	makePretty(deltaTBbarHist,kRed);
	deltaTBHist->Draw("he");
	deltaTBbarHist->Draw("hesame");

	std::cout << "B: " << neventsb << " Bbar: " << neventsbbar << std::endl;
}
