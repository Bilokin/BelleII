
void makePretty(TH1 * x)
{
	x->SetStats(0);
	x->SetLineWidth(2);
}
void Ymc(string filename = "mc.root")
{
	TCanvas * c1 = new TCanvas("c1", "Dalitz",0,0, 1200,400);
	c1->Divide(3,1);
	float tomum = 10000;
	TFile * file = TFile::Open(filename.c_str());
	TTree* Y4S = (TTree*)file->Get("Y4S");
	TH1F * x = new TH1F("x",";X [#mum]",50, -0.003*tomum,0.003*tomum);
	TH1F * y = new TH1F("y",";Y [#mum]",50, -0.00002*tomum,0.00002*tomum);
	TH1F * z = new TH1F("z",";Z [#mum]",50, -0.06*tomum,0.06*tomum);
	makePretty(x);
	makePretty(y);
	makePretty(z);
	c1->cd(1);
	Y4S->Draw("10000*Upsilon4S_TruthX >> x");
	c1->cd(2);
	Y4S->Draw("10000*Upsilon4S_TruthY >> y");
	c1->cd(3);
	Y4S->Draw("10000*Upsilon4S_TruthZ >> z");
	cout << "RMS: " << x->GetRMS()  << " " << y->GetRMS()  << " " << z->GetRMS() << endl;

	Y4S->Draw("10000*Upsilon4S_TruthZ >> z");
	TCanvas * c2 = new TCanvas("c2", "Dalitz", 0, 0, 500, 500);
	TH1F * distanceHist = new TH1F("distanceHist", ";distance []", 50,0,500);
	Y4S->Project("distanceHist", "sqrt( pow(Upsilon4S_B00_TruthX - Upsilon4S_B01_TruthX,2) + pow(Upsilon4S_B00_TruthY - Upsilon4S_B01_TruthY,2) + pow(Upsilon4S_B00_TruthZ - Upsilon4S_B01_TruthZ,2))*10000");
	distanceHist->Draw();
}
