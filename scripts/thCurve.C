void thCurve()
{
	float tauB = 1.52; //e-12 s
	//float deltaM = 3.3e-10;//MeV
	float deltaM = 0.51;//e12 h s^-1
	float S = 0.7;
	float A = 0.1;

	TCanvas *c1 = new TCanvas("c1", "Dalitz", 0, 0, 500, 500);
	TF1 * myBdeltaT = new TF1("myBdeltaT",      "exp(-abs(x)/[0])/(4*[0])*( 1 + ([1]*cos([3]*x)+[2]*sin([3]*x)) )",-10,10);
	TF1 * myBbardeltaT = new TF1("myBbardeltaT","exp(-abs(x)/[0])/(4*[0])*( 1 - ([1]*cos([3]*x)+[2]*sin([3]*x)) )",-10,10);
	//TF1 * myBdeltaT = new TF1("myBdeltaT","exp(-abs(x)/[0])/(4*[0])",-10,10);
	myBdeltaT->SetParameters(tauB, A, S, deltaM);
	myBbardeltaT->SetParameters(tauB, A, S, deltaM);
	myBbardeltaT->SetLineStyle(2);
	std::cout << "P(0) = " << myBdeltaT->Eval(0) << std::endl;
	std::cout << "P(1) = " << myBdeltaT->Eval(1) << std::endl;
	myBdeltaT->Draw();
	myBbardeltaT->Draw("same");
	myBdeltaT->GetXaxis()->SetTitle("#Delta t");
}
