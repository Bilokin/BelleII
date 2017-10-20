void DalitzReco(string filename = "test.root")
{
	TFile * file = TFile::Open(filename.c_str());
	TTree* B0Signal = (TTree*)file->Get("B0Signal");
	TCanvas * c1 = new TCanvas("c1", "Dalitz",0,0, 1200,400);
	TH1F * Kpion = new TH1F("Kpion","M(K^{0} #pi^{#pm});M(K^{0} #pi^{#pm}) [GeV]",50,0.6,1.6);
	TH1F * pionpion = new TH1F("pionpion","M(#pi^{#mp} #pi^{#pm});M(#pi^{#mp} #pi^{#pm}^{#pm}) [GeV]",50,0.2,1.6);
	//TH2F * dalitz = new TH2F("dalitz","Dalitz plot;M(#pi^{#pm} #pi^{#mp}) [GeV];M(K^{0} #pi^{#pm})) [GeV]",50,0.2,1.2,50,0.6,1.2);
	TH2F * dalitz = new TH2F("dalitz","Dalitz plot;M(K^{0} #pi^{#mp}) [GeV];M(#pi^{#pm}  #pi^{#mp})) [GeV]",75,0.6,1.2,75,0.2,1.2);
	
	c1->Divide(3,1);
	string m12 = "B0_m12";//"sqrt(pow(" + E1 + p + E2 + ",2)"+m+ "pow("+px1+p+px2 +",2)"+m+ "pow("+py1+p+py2 +",2)"+m+ "pow("+pz1+p+pz2 +",2)" +")";
	string m01 = "B0_m13";//"sqrt(pow(" + E0 + p + E1 + ",2)"+m+ "pow("+px0+p+px1 +",2)"+m+ "pow("+py0+p+py1 +",2)"+m+ "pow("+pz0+p+pz1 +",2)" +")";
	string m02 = "B0_m23";//"sqrt(pow(" + E0 + p + E2 + ",2)"+m+ "pow("+px0+p+px2 +",2)"+m+ "pow("+py0+p+py2 +",2)"+m+ "pow("+pz0+p+pz2 +",2)" +")";
	//string mccut = "abs(B0_mcPDG) == 511";
	//string mccut = "B0_isSignal == 1";
	//string mccut = "1";
	string mccut = "B0_mbc > 5.27 && B0_deltae > -0.2 && B0_deltae < 0.1"; // BEST

	c1->cd(1);
	//K1Signal->Draw( (m12).c_str() );
	B0Signal->Project("pionpion", m12.c_str(), mccut.c_str());
	pionpion->Draw();
	gPad->SetLeftMargin(0.14);
	gPad->SetBottomMargin(0.14);
	pionpion->GetYaxis()->SetTitleOffset(1.3);
	pionpion->GetXaxis()->SetTitleOffset(1.3);
	c1->cd(2);
	//K1Signal->Draw( m01.c_str() );
	B0Signal->Project("Kpion", m02.c_str(), mccut.c_str());
	Kpion->Draw();
	gPad->SetLeftMargin(0.14);
	gPad->SetBottomMargin(0.14);
	Kpion->GetYaxis()->SetTitleOffset(1.3);
	Kpion->GetXaxis()->SetTitleOffset(1.3);
	c1->cd(3);
	//K1Signal->Draw( (m01+":"+m12).c_str(),"","colz" );
	B0Signal->Project("dalitz", (m12+":"+m02).c_str(), mccut.c_str());
	dalitz->Draw("colz");
	gPad->SetLeftMargin(0.14);
	gPad->SetBottomMargin(0.14);
	dalitz->GetYaxis()->SetTitleOffset(1.3);
	dalitz->GetXaxis()->SetTitleOffset(1.3);
	TF1 * cutf = new TF1("cutf","-0.8+1./0.6*x",0.6,1.2);
	//cutf->Draw("same");
}


