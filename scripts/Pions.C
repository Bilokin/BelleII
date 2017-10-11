

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
	//htemp->SetStats(0);
}
void Pions(string filename = "test.root")
{
	gROOT->Reset(); 
	int nevents = 5000;
	TFile * file = TFile::Open(filename.c_str());
	//TTree* K0Signal = (TTree*)file->Get("K0Signal");
	TTree* pions = (TTree*)file->Get("pions");
	TCanvas * c1 = new TCanvas("c1", "Dalitz",0,0, 1200,400);

	c1->Divide(3,1);

	c1->cd(1);
        TH1F* EresPiPi = new TH1F("EresPiPi",";|p_{#pi#pi}| resolution, %",50,0,2);
        TH1F* EresK0 = new TH1F("EresK0",";|p_{K^{0}}| resolution, %",50,0,2);
	//string cutPiPi = "abs(pi_MC_GD_MOTHER_ID) == 10313 && abs(pi_MC_GD_GD_MOTHER_ID) == 511 && (abs(pi_MC_MOTHER_ID) == 323 || abs(pi_MC_MOTHER_ID) == 113)";
	string cutPiPi = "(abs(pi_MC_GD_MOTHER_ID) == 10313 && abs(pi_MC_GD_GD_MOTHER_ID) == 511 && (abs(pi_MC_MOTHER_ID) == 323 || abs(pi_MC_MOTHER_ID) == 113)) || (abs(pi_MC_MOTHER_ID) == 10313 && abs(pi_MC_GD_MOTHER_ID) == 511)";
	string cutK0 = "((abs(pi_MC_GD_MOTHER_ID) == 10313 && abs(pi_MC_GD_GD_MOTHER_ID) == 511)|| (abs(pi_MC_GD_GD_MOTHER_ID) == 10313)) && (abs(pi_MC_MOTHER_ID) == 310)";
	int nrecoPiPi = pions->Draw("pi_P",(cutPiPi).c_str());
	//int nrecoK0 = pions->Draw("pi_P",(cutK0 + " && pi_nSVDHits > 0").c_str());
	int nrecoK0 = pions->Draw("pi_P",(cutK0 + " ").c_str());
	cout << "N pi+ pi- reconstructed: " << nrecoPiPi << " (" << (float)nrecoPiPi/nevents/2*100 << "%)\n";
	cout << "N K0S reconstructed: " << nrecoK0 << " (" << (float)nrecoK0/nevents/2*100/0.7 << "%)\n";

		
	pions->Project("EresPiPi","abs(pi_P - pi_TruthP)/pi_TruthP*100", (cutPiPi + "&&abs(pi_P - pi_TruthP)/pi_TruthP*100 < 10").c_str());// && (pi__cosTheta > 0.85 || pi__cosTheta < -0.65)").c_str());
	makePretty(EresPiPi);
	EresPiPi->Draw("he");
	//pions->Draw("abs(pi_P - pi_TruthP)/pi_TruthP*100:pi__cosTheta", (cutPiPi + "&&abs(pi_P - pi_TruthP)/pi_TruthP*100 < 2").c_str());
	pions->Project("EresK0","abs(pi_P - pi_TruthP)/pi_TruthP*100", (cutK0 + "&&abs(pi_P - pi_TruthP)/pi_TruthP*100 < 10").c_str());
	makePretty(EresK0,kGreen);
	EresK0->Draw("hesame");
	cout << "E B0 pions resolution: " << EresPiPi->GetRMS() << "%\n";
	cout << "E K0 pions resolution: " << EresK0->GetRMS() << "%\n";
	TLegend *legendMean2 = new TLegend(0.45,0.65,0.85,0.8,NULL,"brNDC");
	legendMean2->SetFillColor(0);
	legendMean2->SetBorderSize(0);
	legendMean2->AddEntry(EresK0,"#pi^{#pm} #pi^{#mp} from K^{0}","f");
	legendMean2->AddEntry(EresPiPi,"#pi^{#pm} #pi^{#mp} from B^{0}","f");
	legendMean2->Draw();
	
	c1->cd(2);
	string newcut = "&& 1 ";// pi_nPXDHits == 0";		
        TH1F* devZPiPi = new TH1F("devZPiPi",";Parameter deviation",50,0,25);
        TH1F* devZK0 = new TH1F("devZK0",";Parameter deviation",50,0,50);
	pions->Project("devZPiPi","abs(pi_z0)/pi_z0Err", (cutPiPi + newcut).c_str());
	pions->Project("devZK0","abs(pi_z0)/pi_z0Err", (cutK0).c_str());// && (pi__cosTheta > 0.85 || pi__cosTheta < -0.65)").c_str());
        TH1F* devDPiPi = new TH1F("devDPiPi",";Parameter deviation",50,0,25);
        TH1F* devDK0 = new TH1F("devDK0",";Parameter deviation",50,0,50);
	pions->Project("devDPiPi","abs(pi_d0)/pi_d0Err", (cutPiPi+newcut).c_str());
	pions->Project("devDK0","abs(pi_d0)/pi_d0Err", (cutK0).c_str());// && (pi__cosTheta > 0.85 || pi__cosTheta < -0.65)").c_str());
	makePretty(devZPiPi);
	makePretty(devDPiPi, kGreen);
	devDPiPi->Draw("he");
	devZPiPi->Draw("hesame");
	TLegend *legendMean = new TLegend(0.4,0.65,0.85,0.8,NULL,"brNDC");
	legendMean->SetFillColor(0);
	legendMean->SetBorderSize(0);
	legendMean->AddEntry(devZPiPi, "Z_{0} of #pi^{#pm} #pi^{#mp} from B^{0}","f");
	legendMean->AddEntry(devDPiPi, "D_{0} of #pi^{#pm} #pi^{#mp} from B^{0}","f");
	legendMean->Draw();
	c1->cd(3);
	makePretty(devZK0);
	makePretty(devDK0, kGreen);
	devZK0->Draw("he");
	devDK0->Draw("hesame");
	TLegend *legendMean3 = new TLegend(0.4,0.65,0.85,0.8,NULL,"brNDC");
	legendMean3->SetFillColor(0);
	legendMean3->SetBorderSize(0);
	legendMean3->AddEntry(devZPiPi, "Z_{0} of #pi^{#pm} #pi^{#mp} from K^{0}","f");
	legendMean3->AddEntry(devDPiPi, "D_{0} of #pi^{#pm} #pi^{#mp} from K^{0}","f");
	legendMean3->Draw();

	TCanvas * c2 = new TCanvas("c2", "Dalitz", 0, 0, 500, 500);
	string selection = "&& 10*sqrt(pi_TruthVtxProd[0]*pi_TruthVtxProd[0] + pi_TruthVtxProd[1]*pi_TruthVtxProd[1]) < 200";
	TH1F* rhoK0 = new TH1F("rhoK0",";#rho [mm]",50,0,200);
	pions->Project("rhoK0","10*sqrt(pi_TruthVtxProd[0]*pi_TruthVtxProd[0] + pi_TruthVtxProd[1]*pi_TruthVtxProd[1])", (cutK0 + selection).c_str());
	makePretty(rhoK0);
	rhoK0->Draw();


}
