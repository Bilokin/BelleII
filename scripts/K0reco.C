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

}
void K0reco(string filename = "test.root")
{
	TFile * file = TFile::Open(filename.c_str());
	TTree* K0Signal = (TTree*)file->Get("K0Signal");
	TTree* pions = (TTree*)file->Get("pions");
	TCanvas * c1 = new TCanvas("c1", "Dalitz",0,0, 1200,400);

	c1->Divide(3,1);
	c1->cd(1);
	int allK = K0Signal->Draw("K_S0_mcPDG", "abs(K_S0_mcPDG) > -1");
	float trueK = K0Signal->Draw("K_S0_mcPDG","abs(K_S0_mcPDG) == 310");
	int totalEntries = 1000;
	K0Signal->Draw("K_S0_P","abs(K_S0_mcPDG) > -1");
	K0Signal->Draw("K_S0_P","abs(K_S0_mcPDG) == 310","same");
	c1->cd(2);
	K0Signal->Draw("K_S0_M","abs(K_S0_mcPDG) > -1");
	K0Signal->Draw("K_S0_M","abs(K_S0_mcPDG) == 310","same");
	c1->cd(3);
	K0Signal->Draw("K_S0_Rho*10","abs(K_S0_mcPDG) > -1 && K_S0_Rho < 20");
	K0Signal->Draw("K_S0_Rho*10","abs(K_S0_mcPDG) == 310 && K_S0_Rho < 20","same");
	cout << "All K0: " << allK << "; true K0: " << trueK << "\n"; 
	cout << "Purity K0: " << trueK/allK*100 << "%\n";
	
	TCanvas * c2 = new TCanvas("c2", "Dalitz",0,0, 1200,400);
	c2->Divide(3,1);
	c2->cd(1);
	TH1F* d0hK = new TH1F("d0hK",";#sigma_{d0}",50,0,50);
	TH1F* d0hpi = new TH1F("d0hpi",";#sigma_{d0}",50,0,50);
	TH1F* d0eff = new TH1F("d0eff",";#sigma_{d0}",50,0,0.5);
	TH1F* z0hK = new TH1F("z0hK",";Parameter deviation",50,0,50);
	TH1F* z0hpi = new TH1F("z0hpi",";#sigma_{z0}",50,0,50);
	TH1F* z0eff = new TH1F("z0eff",";#sigma_{z0}",50,0,0.5);
	TH1F* coshK = new TH1F("coshK",";cos",70,-1,1);
	TH1F* coshpi = new TH1F("coshpi",";cos",70,-1,1);
	TH1F* coseff = new TH1F("coseff",";cos",70,-1,1);
	TH1F* phK = new TH1F("phK",";p",50,0,1);
	TH1F* phpi = new TH1F("phpi",";p",50,0,1);
	TH1F* peff = new TH1F("peff",";p",50,0,1);
	float offsetm = 50;
	TH1F* offsethK = new TH1F("offsethK",";p",50,0,offsetm);
	TH1F* offsethpi = new TH1F("offsethpi",";p",50,0,offsetm);
	TH1F* offseteff = new TH1F("offseteff",";p",50,0,offsetm);
	TH1F* seenhK = new TH1F("seenhK",";seen",2,0,2);
	TH1F* seenhpi = new TH1F("seenhpi",";seen",2,0,2);
	TH1F* seeneff = new TH1F("seeneff",";seen",2,0,2);
	d0hK->Sumw2();
	d0hpi->Sumw2();
	z0hK->Sumw2();
	z0hpi->Sumw2();
	coshK->Sumw2();
	coshpi->Sumw2();
	phK->Sumw2();
	phpi->Sumw2();
	seenhK->Sumw2();
	seenhpi->Sumw2();
	offsethK->Sumw2();
	offsethpi->Sumw2();


	K0Signal->Project( "d0hK","abs(K_S0_pi0__d0)/K_S0_pi0__d0Err","K_S0_pi0__d0Err < 1 && K_S0_mcPDG == 310");
	K0Signal->Project("+d0hK","abs(K_S0_pi1__d0)/K_S0_pi1__d0Err","K_S0_pi1__d0Err < 1 && K_S0_mcPDG == 310");
	pions->Project("d0hpi","pi__d0Err","abs(pi__genMotherPDG) == 310 && pi__d0Err < 1");
	d0eff->Divide(d0hK,d0hpi);
	//makePretty(d0hK);
	//d0hpi->Draw();
	makePretty(d0hK, kGreen);
	//d0eff->Draw("e");
	K0Signal->Project( "z0hK","abs(K_S0_pi0__z0)/K_S0_pi0__z0Err","K_S0_pi0__z0Err < 1 && K_S0_mcPDG == 310");
	K0Signal->Project("+z0hK","abs(K_S0_pi1__z0)/K_S0_pi1__z0Err","K_S0_pi1__z0Err < 1 && K_S0_mcPDG == 310");
	pions->Project("z0hpi","pi__z0Err","abs(pi__genMotherPDG) == 310 && pi__z0Err < 1");
	z0eff->Divide(z0hK,z0hpi);
	//makePretty(z0hK);
	//z0hpi->Draw();
	makePretty(z0hK);
	z0hK->Draw("he");
	d0hK->Draw("hesame");
	TLegend *legendMean3 = new TLegend(0.4,0.65,0.85,0.8,NULL,"brNDC");
	legendMean3->SetFillColor(0);
	legendMean3->SetBorderSize(0);
	legendMean3->AddEntry(z0hK, "Z_{0} of #pi^{#pm} #pi^{#mp} from K^{0}","f");
	legendMean3->AddEntry(d0hK, "D_{0} of #pi^{#pm} #pi^{#mp} from K^{0}","f");
	legendMean3->Draw();
	c2->cd(2);
	//z0eff->Draw("e");
	c2->cd(3);
	K0Signal->Project("coshK","K_S0_pi0__cosTheta","K_S0_mcPDG == 310");
	K0Signal->Project("+coshK","K_S0_pi1__cosTheta","K_S0_mcPDG == 310");
	pions->Project("coshpi","pi__cosTheta","abs(pi__genMotherPDG) == 310");

	K0Signal->Project("phK","abs(K_S0_pi0_P-K_S0_pi0_TruthP)/K_S0_pi0_TruthP*100","K_S0_mcPDG == 310");
	K0Signal->Project("+phK","abs(K_S0_pi1_P-K_S0_pi1_TruthP)/K_S0_pi1_TruthP*100","K_S0_mcPDG == 310");
	pions->Project("phpi","abs(pi_P - pi_TruthP)/pi_TruthP*100","abs(pi__genMotherPDG) == 310 &&abs(pi_mcPDG) == 211");

	/*K0Signal->Project("offsethK","K_S0_pi0__d0/K_S0_pi0__d0Err+abs(K_S0_pi0__z0)/K_S0_pi0__z0Err","K_S0_mcPDG == 310");
	K0Signal->Project("+offsethK","K_S0_pi1__d0/K_S0_pi1__d0Err + abs(K_S0_pi1__z0)/K_S0_pi1__z0Err","K_S0_mcPDG == 310");
	pions->Project("offsethpi","pi__d0/pi__d0Err+abs(pi__z0)/pi__z0Err","abs(pi__genMotherPDG) == 310");*/
	K0Signal->Project("offsethK","K_S0_pi0__z0/K_S0_pi0__z0Err","K_S0_mcPDG == 310");
	K0Signal->Project("+offsethK","K_S0_pi1__z0/K_S0_pi1__z0Err","K_S0_mcPDG == 310");
	pions->Project("offsethpi","pi__z0/pi__z0Err","abs(pi__genMotherPDG) == 310");//*/
	
	coseff->Divide(coshK,coshpi);
	coshpi->Draw();
	coshK->Draw("same");
	coseff->Draw("e");

	K0Signal->Project("seenhK","K_S0_pi0_SeenInECL","K_S0_mcPDG == 310");
	K0Signal->Project("+seenhK","K_S0_pi1_SeenInECL","K_S0_mcPDG == 310");
	pions->Project("seenhpi","pi_SeenInECL","abs(pi__genMotherPDG) == 310");
	seeneff->Divide(seenhK,seenhpi);
	seenhpi->Draw();
	seenhK->Draw("same");
	seeneff->Draw("e");
	peff->Divide(phK,phpi);
	phpi->Draw();
	phK->Draw("same");
	//peff->Draw("e");
	
	offseteff->Divide(offsethK,offsethpi);
	offsethpi->Draw();
	offsethK->Draw("same");
	//offseteff->Draw("e");//*/
}
