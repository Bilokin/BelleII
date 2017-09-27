
void makePretty(TH1F* htemp)
{
	htemp->SetLineWidth(3);
	htemp->SetFillStyle(3004);
	htemp->SetFillColor(kBlue);
	htemp->SetLineColor(kBlue);
	htemp->SetStats(0);
}
void GammaReco(string filename = "test.root")
{
	TFile * file = TFile::Open(filename.c_str());
	TTree* GammaSignal = (TTree*)file->Get("GammaSignal");
	TCanvas * c1 = new TCanvas("c1", "Dalitz",0,0, 800,400);
        TH1F* Eres = new TH1F("Eres",";E_{gamma} resolution, %",50,0,20);
        TH1F* Cos = new TH1F("Cos",";cos#theta",75,-1,1);

	c1->Divide(2,1);
	c1->cd(1);
	GammaSignal->Project("Eres","abs(gamma_P-gamma_TruthP)/gamma_TruthP*100","abs(gamma_P-gamma_TruthP)/gamma_TruthP < 0.2");
	cout << "E resolution: " << Eres->GetRMS() << "%\n";
	makePretty(Eres);
	Eres->Draw("eh");
	//GammaSignal->Draw("abs(gamma_P-gamma_TruthP)/gamma_TruthP:gamma_P","abs(gamma_P-gamma_TruthP)/gamma_TruthP < 0.2");
	c1->cd(2);
	GammaSignal->Project("Cos","gamma__cosTheta","");
	makePretty(Cos);
	Cos->Draw("eh");
}
