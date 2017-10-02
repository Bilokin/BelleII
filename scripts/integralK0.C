
void integralK0(string filename = "mc.root")
{
	TFile * file = TFile::Open(filename.c_str());
	TTree* K0sMy = (TTree*)file->Get("K0sMy");
	TH1F * k0Hist = new TH1F("k0Hist", ";#rho []", 200,0,200);
	TH1F * k0Hist2 = new TH1F("k0Hist2", ";#rho [mm]", 200,0,200);
	K0sMy->Project("k0Hist","K_S0_TruthRho*10","K_S0_TruthRho < 20 && abs(K_S0_MC_MOTHER_ID) == 10313 || abs(K_S0_MC_MOTHER_ID) == 323");
	//K0sMy->Project("k0Hist","K_S0_TruthRho*10","K_S0_TruthRho < 20");
	k0Hist->Draw();
	float all = k0Hist->Integral(1,200);
	float pxd = k0Hist->Integral(1,22);
	float svd1 = k0Hist->Integral(23,38);
	float svd2 = k0Hist->Integral(39,80);
	float svd3 = k0Hist->Integral(81,115);
	float svd4 = k0Hist->Integral(116,140);
	float nosvd = k0Hist->Integral(141,200);
	std::cout << "All: " << all << " pxd: " << pxd  << " svd1: " << svd1 << std::endl;
	std::cout << "Check: " << all- svd1-pxd - svd2-svd3-svd4-nosvd << std::endl;
	float x[6] = {11., (38.-22)/2+22, (80.-38)/2+38, (115.-80)/2+80, (140.-115)/2+115, (200-140)/2+140};
	float y[6] = {pxd/all, svd1/all, svd2/all, svd3/all, svd4/all, nosvd/all};
	float sx[6] = {11., (38.-22)/2, (80.-38)/2, (115.-80)/2, (140.-115)/2, (200.-140)/2};
	float sy[6] = {sqrt(pxd)/all, sqrt(svd1)/all, sqrt(svd2)/all, sqrt(svd3)/all, sqrt(svd4)/all, sqrt(nosvd)/all};
	TGraphErrors * graph = new TGraphErrors(6,x,y,sx,sy);
	graph->SetLineColor(kRed);
	graph->SetLineWidth(3);
	graph->SetMarkerStyle(21);
	graph->SetMarkerColor(kRed);
	k0Hist2->SetMaximum(0.75);
	k0Hist2->Draw();
	graph->Draw("p");
}
