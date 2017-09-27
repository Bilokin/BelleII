void K1reco(string filename = "test.root")
{
	TCanvas * c1 = new TCanvas("c1", "Dalitz",0,0, 1200,400);

	c1->Divide(3,1);
	c1->cd(1);
	TFile * file = TFile::Open(filename.c_str());
	int allK = K1Signal->Draw("K_10_mcPDG", "abs(K_10_mcPDG) > -1");
	float trueK = K1Signal->Draw("K_10_mcPDG","abs(K_10_mcPDG) == 10313");
	int totalEntries = 1000;
	K1Signal->Draw("K_10_P","abs(K_10_mcPDG) > -1");
	K1Signal->Draw("K_10_P","abs(K_10_mcPDG) == 10313","same");
	c1->cd(2);
	K1Signal->Draw("K_10_M","abs(K_10_mcPDG) > -1");
	K1Signal->Draw("K_10_M","abs(K_10_mcPDG) == 10313","same");
	c1->cd(3);
	K1Signal->Draw("K_10_Rho","abs(K_10_mcPDG) > -1 && K_10_Rho < 10");
	K1Signal->Draw("K_10_Rho","abs(K_10_mcPDG) == 10313 && K_10_Rho < 10","same");
	cout << "All K10: " << allK << "; true K0: " << trueK << "\n"; 
	cout << "Purity K10: " << trueK/allK*100 << "%\n"; 
}
