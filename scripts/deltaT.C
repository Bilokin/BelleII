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
TH1F * drawHists(TTree* B0Signal, string name, string cut, string title = "Err", float r1 = 0, float r2 = 1, string mccut =  "B0_isSignal  &&")
{
	int nbins = 100;	
	string observable = name;
	string histname = name;
	if (histname.size() > 10) 
	{
		histname =  name.substr(1,5)+title.substr(0,4);
	}
	TH1F * xPullHist = new TH1F((histname+"Hist").c_str(), (";"+title).c_str(), nbins,r1, r2);
	TH1F * xPullAllHist = new TH1F((histname+"AllHist").c_str(), (";"+title).c_str(), nbins, r1, r2);
	
	B0Signal->Project((histname+"Hist").c_str(), observable.c_str(),(mccut+cut).c_str());
	B0Signal->Project((histname+"AllHist").c_str(), observable.c_str(),(cut).c_str(), "same");
	
	makePretty(xPullHist);
	makePretty(xPullAllHist, kGray+1);
	
	xPullAllHist->Draw("he");
	xPullHist->Draw("hesame");
	return xPullHist;
}

void fillWhist(TH1F* wHist, TTree* B0Signal, fitSettings & settings, string cut)
{       
	string more = "abs(B0_FBDT_qrCombined) > ";
	string less = "abs(B0_FBDT_qrCombined) < ";
	string ands = " && ";
	for (unsigned int i = 0; i < settings.wvalues.size(); i++) 
	{
		string selection = more+to_string(settings.qbins[i]) +ands + less + to_string(settings.qbins[i+1]);
		int number = B0Signal->GetEntries((cut+ands+selection).c_str());
		std::cout << "W: " << settings.wvalues[i] << " Number: " << number << " Error: " << sqrt((float)number) <<" selection: " << selection<< std::endl;
		//wHist->Fill(settings.wvalues[i], number);
		//wHist->SetBinError(i+1, sqrt((float)number));
		wHist->SetBinContent(settings.wvalues.size() - i, number);	
	}
	
}

fitSettings deltaT(string filename = "test.root", string Kres = "Xsd", string cut = "")
{
	bool gen =false;
	std::cout << filename << std::endl;
	fitSettings fitResult;
	if (cut == "") 
	{
		cut = getSignalCuts(0, Kres);
	}
	//cut += " && B0_CSMVA > 0.1";
	string trueB = "&& (B0_isSignal)";
	string bkgB = "&& B0_isSignal == 0";

	TCanvas * c1 = new TCanvas("c1", "Dalitz",0,0, 1500,1000);
	c1->Divide(3,2);
	TFile * file = TFile::Open(filename.c_str());
	TTree* B0Signal = (TTree*)file->Get("B0Signal");
	/////////////////////////////////////////////////////////////////////////////////////////////////////////
	/////////////////////////////////////////////////////////////////////////////////////////////////////////
	/////////////////////////////////////////////////////////////////////////////////////////////////////////
	c1->cd(1);
	TH1F * deltaTBbkgHist = new TH1F("deltaTBbkgHist", ";#Delta t [ps]", 100,-10,10);
	float neventsbkg = B0Signal->Project("deltaTBbkgHist", "(B0_DeltaT)", (cut + bkgB).c_str());
	makePretty(deltaTBbkgHist, kGreen +1);
	TH1F * deltaTBtrueHist = drawHists(B0Signal, "B0_DeltaT", cut, "#Deltat [ps]",-10,10);
	float neventstrue =  deltaTBtrueHist->GetEntries();
	float nevents = deltaTBtrueHist->GetEntries() + neventsbkg;
	deltaTBbkgHist->Draw("same");
	//TF1 * deltat = new TF1("deltat","[0]*TMath::Gaus(x,[1],[2])+ [3]*TMath::Gaus(x,0,[4])",-10,10);	
	TF1 * bkgPdf = new TF1("bkgPdf","[0]*TMath::Gaus(x,0,[1],1)+ [2]*TMath::Gaus(x,0,[3],1)",-10,10);
	bkgPdf->SetParLimits(1,0.1,10);
	bkgPdf->SetParLimits(3,0.01,2);
	bkgPdf->SetLineColor(kGreen);
	deltaTBbkgHist->Fit("bkgPdf");
	float fbkg = bkgPdf->GetParameter(0)/(bkgPdf->GetParameter(2)+bkgPdf->GetParameter(0));
	fitResult.fbkg.push_back(bkgPdf->GetParameter(0)/(bkgPdf->GetParameter(2)+bkgPdf->GetParameter(0)));
	fitResult.fbkg.push_back(bkgPdf->GetParameter(2)/(bkgPdf->GetParameter(2)+bkgPdf->GetParameter(0)));
	fitResult.fsig = neventstrue/nevents;
	fitResult.sigmabkg.push_back(bkgPdf->GetParameter(1));
	fitResult.sigmabkg.push_back(bkgPdf->GetParameter(3));
	//coreT->Draw("same");
	c1->cd(2);
	
	TH1F * deltaTPullHist = drawHists(B0Signal, "(B0_DeltaT-B0_TruthDeltaT)/B0_DeltaTErr", cut, "pull #Deltat [sigma]",-10,10);
	TF1 * pullPdf = new TF1("pullPdf","[0]*TMath::Gaus(x,0,1,1)",-10,10);
	pullPdf->SetLineColor(kOrange);
	pullPdf->SetLineStyle(9);
	pullPdf->SetLineWidth(5);
	deltaTPullHist->Fit("pullPdf");
	/////////////////////////////////////////////////////////////////////////////////////////////////////////
	/////////////////////////////////////////////////////////////////////////////////////////////////////////
	/////////////////////////////////////////////////////////////////////////////////////////////////////////
	c1->cd(3);
	int nbinsf = 50;
	TH1F * tagBtrueHist = new TH1F("tagBtrueHist", ";Flavour tag output (FBDT) []; Purity", nbinsf,-1,1);
	TH1F * tagBAllHist = new TH1F("tagBAllHist", ";FBDT []", nbinsf,-1,1);
	TH1F * tagBbartrueHist = new TH1F("tagBbartrueHist", ";FBDT []", nbinsf,-1,1);
	const int size = 15;
	Float_t xbins[size] = {-1, -0.875, -0.75, -0.625, -0.5, -0.25, -0.1,  0, 0.1, .25, 0.5, .625, .75, .875, 1};
	TH1F * tagBtrueRebinHist = new TH1F("tagBtrueRebinHist", ";Flavour tag output (FBDT) []; Purity", size-1,xbins);
	TH1F * tagBAllRebinHist = new TH1F("tagBAllRebinHist", ";FBDT []", size-1,xbins);
	TH1F * tagBbartrueRebinHist = new TH1F("tagBbartrueRebinHist", ";FBDT []", size-1,xbins);
	tagBtrueHist->Sumw2();
	tagBbartrueHist->Sumw2();
	tagBAllHist->Sumw2();
	tagBtrueRebinHist->Sumw2();
	tagBbartrueRebinHist->Sumw2();
	tagBAllRebinHist->Sumw2();
	B0Signal->Project("tagBtrueHist", "B0_FBDT_qrCombined", (cut + trueB + "&& B0_qrMC == 1").c_str());
	B0Signal->Project("tagBAllHist", "B0_FBDT_qrCombined", (cut+trueB+ "&& abs(B0_qrMC)  == 1").c_str());
	B0Signal->Project("tagBbartrueHist", "B0_FBDT_qrCombined", (cut + trueB + "&& B0_qrMC == -1").c_str());
	B0Signal->Project("tagBtrueRebinHist", "B0_FBDT_qrCombined", (cut + trueB + "&& B0_qrMC == 1").c_str());
	B0Signal->Project("tagBAllRebinHist", "B0_FBDT_qrCombined", (cut+trueB+ "&& abs(B0_qrMC)  == 1").c_str());
	B0Signal->Project("tagBbartrueRebinHist", "B0_FBDT_qrCombined", (cut + trueB + "&& B0_qrMC == -1").c_str());
	//wHist->Divide(tagBbartrueHist, tagBAllHist);
	//tagBtrueHist->Divide(tagBAllHist);
	//tagBbartrueHist->Divide(tagBAllHist);
	tagBtrueRebinHist->Divide(tagBAllRebinHist);
	tagBbartrueRebinHist->Divide(tagBAllRebinHist);
	makePretty(tagBtrueHist);
	makePretty(tagBAllHist, kGray+1);
	makePretty(tagBbartrueHist,kRed);
	makePretty(tagBtrueRebinHist,kBlue);
	makePretty(tagBAllRebinHist, kGray+1);
	makePretty(tagBbartrueRebinHist,kRed);
	//tagBAllHist->Draw();
	//tagBtrueHist->Draw("samehe");
	//tagBbartrueHist->Draw("samehe");
	tagBtrueRebinHist->Draw("samehe");
	tagBbartrueRebinHist->Draw("samehe");
	float qlow = 0.5; float qhigh = 1.;
	TF1 * wFunc = new TF1("wFunc","[0]+[1]*x",0,1);
	TF1 * wFuncbar = new TF1("wFuncbar","[0]+[1]*x",0,1);
	wFuncbar->SetLineColor(kRed+1);
	wFunc->SetLineColor(kBlue+1);
	
	for (int i = 0; i < 7; i++) 
	{
		std::cout << i << std::endl;
		fitResult.wvalues.push_back(tagBbartrueRebinHist->GetBinContent(i+8)); //Need to rewrite this, only half of the data used
	}
	tagBbartrueRebinHist->Fit(wFuncbar);
	tagBtrueRebinHist->Fit(wFunc);
	//float wratio = wFunc->Integral(0,1)/ wFuncbar->Integral(-1,0);
	float wratio = tagBtrueRebinHist->Integral(8,15)/ tagBbartrueRebinHist->Integral(1,7);
	std::cout << "wratio: " << wratio  << std::endl;
	/////////////////////////////////////////////////////////////////////////////////////////////////////////
	/////////////////////////////////////////////////////////////////////////////////////////////////////////
	/////////////////////////////////////////////////////////////////////////////////////////////////////////
	c1->cd(4);
	TH1F * deltaTResHist = drawHists(B0Signal, "(B0_DeltaT-B0_TruthDeltaT)", cut, "Residual #Deltat [ps]",-10,10);

	deltaTResHist->Draw("samehe");
	//TF1 * deltatRes = new TF1("deltatRes","gaus+gaus(3)",-10,10);
	//TF1 * deltatRes = new TF1("deltatRes","[0]*TMath::Gaus(x,[1],[2]) + (1.-[0])*([3]*TMath::Gaus(x,[4],[5]) + (1.-[3]) * TMath::Gaus(x,[6],[7]) )",-10,10);
	//TF1 * deltatRes = new TF1("deltatRes","[0]*TMath::Gaus(x,[1],[2]) + [3]*TMath::Gaus(x,[4],[5]) + [6] * TMath::Gaus(x,[7],[8])",-10,10);
	TF1 * deltatRes = new TF1("deltatRes","[0]*TMath::Gaus(x,0,[1],1) + [2]*TMath::Gaus(x,0,[3],1) + [4] * TMath::Gaus(x,0,[5],1)",-10,10);
	makePretty(deltatRes, kBlue+1);
	//deltatRes->SetParLimits(0, nevents/1000, nevents);
	//deltatRes->SetParLimits(1, -0.1, 0.1);
	deltatRes->SetParLimits(1, 0.1, 2);
	//deltatRes->SetParLimits(3, nevents/1000, nevents/20);
	//deltatRes->SetParLimits(4, -1, 1);
	deltatRes->SetParLimits(3, 0.1, 5);
	//deltatRes->SetParLimits(7, -0.2, 0.2);
	deltatRes->SetParLimits(5, 0.01, 5);
	deltaTResHist->Fit("deltatRes");
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
	fitResult.fres.push_back(deltatRes->GetParameter(0)/(deltatRes->GetParameter(2)+deltatRes->GetParameter(0) + deltatRes->GetParameter(4)));
	fitResult.fres.push_back(deltatRes->GetParameter(2)/(deltatRes->GetParameter(2)+deltatRes->GetParameter(0) + deltatRes->GetParameter(4)));
	fitResult.fres.push_back(deltatRes->GetParameter(4)/(deltatRes->GetParameter(2)+deltatRes->GetParameter(0) + deltatRes->GetParameter(4)));
	fitResult.sigmares.push_back(deltatRes->GetParameter(1));
	fitResult.sigmares.push_back(deltatRes->GetParameter(3));
	fitResult.sigmares.push_back(deltatRes->GetParameter(5));
	//core->Draw("same");
	//bkg1->Draw("same");
	//bkg2->Draw("same");
	/////////////////////////////////////////////////////////////////////////////////////////////////////////
	/////////////////////////////////////////////////////////////////////////////////////////////////////////
	/////////////////////////////////////////////////////////////////////////////////////////////////////////
	c1->cd(6);
	float wreal = 1- tagBtrueHist->Integral(26,51)/(tagBtrueHist->Integral(26,51)+tagBbartrueHist->Integral(26,51));
	std::cout << "B true: " << tagBtrueHist->Integral(26,51) << std::endl;
	std::cout << "Bbar true: " << tagBbartrueHist->Integral(26,51) << std::endl;
	const int wsize = 8;
	Float_t wbins[size] = { 0, 0.05, .1, 0.18, .22, .35, .45, .5};
	TH1F * wHistRebinned = new TH1F("wHistRebinned", ";w []", wsize-1,wbins);
	TH1F * wHist = new TH1F("wHist", ";w []", nbinsf,0,0.5);
	
	fillWhist(wHistRebinned, B0Signal, fitResult, cut+trueB);
	
	TH1F * wHistBkg = new TH1F("wHistBkg", ";w []", nbinsf,0,0.5);
	fitResult.wparameters = {0.5, -0.5};
//	fitResult.wparameters = {(float)wFuncbar->GetParameter(0), (float)wFuncbar->GetParameter(1)};
	B0Signal->Project("wHist", ("abs(" + to_string(fitResult.wparameters[0]) +"+"+ to_string(fitResult.wparameters[1]) + "*abs(B0_FANN_qrCombined))").c_str(), (cut + trueB +  "&& abs(B0_qrMC) == 1").c_str());
	//B0Signal->Project("wHistRebinned", ("abs(" + to_string(fitResult.wparameters[0]) +"+"+ to_string(fitResult.wparameters[1]) + "*abs(B0_FANN_qrCombined))").c_str(), (cut + trueB +  "&& abs(B0_qrMC) == 1").c_str());
	B0Signal->Project("wHistBkg", ("abs(" + to_string(fitResult.wparameters[0]) +"+"+ to_string(fitResult.wparameters[1]) + "*abs(B0_FANN_qrCombined))").c_str(), (cut + bkgB ).c_str());
	//wHist->Scale(1./wHist->GetEntries());
	//wHistRebinned->Scale(wHist->GetMaximum()/wHistRebinned->GetMaximum());
	makePretty(wHist);
	makePretty(wHistBkg,kGreen);
	makePretty(wHistRebinned, kBlue,0);
	fitResult.w = wreal;// std::cout << "-----------------\nCRUNCH: w is not a real value!!!!\n--------------------" << std::endl;
	fitResult.dw = -abs(1-wratio);
	TF1 * wPDF = new TF1("wPDF", "pol5",0.0,0.5);
	TF1 * wPDFbkg = new TF1("wPDFbkg", "pol5",0.0,0.5);
	wPDF->SetLineColor(kBlue+1);
	wPDFbkg->SetLineColor(kGreen+1);
	wHist->Draw("he");
	wHistRebinned->Draw("hesame");
	wHistBkg->Draw("hesame");
	wHist->Fit("wPDF","QR");
	wHistBkg->Fit("wPDFbkg","QR");
	/////////////////////////////////////////////////////////////////////////////////////////////////////////
	/////////////////////////////////////////////////////////////////////////////////////////////////////////
	/////////////////////////////////////////////////////////////////////////////////////////////////////////
	c1->cd(5);
	TH1F * deltaTErrHist = drawHists(B0Signal, "(B0_DeltaTErr)", cut, "Uncertainty #Deltat [ps]",0,5);
	//deltaTErrHist->Fit("landau");
	/////////////////////////////////////////////////////////////////////////////////////////////////////////
	if (gen) 
	{
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
	}
		std::cout << "-------------------------------------------------"  << std::endl;
	cout <<  "nEvents: " << nevents << " signal fraction: " << fitResult.fsig << endl;
	fitResult.Print();
	std::cout << "W signal function:" << std::endl;
	std::cout << wPDF->GetExpFormula("p") << std::endl;
	std::cout << "W bkg function:" << std::endl;
	std::cout << wPDFbkg->GetExpFormula("p") << std::endl;
	//std::cout << "Purity: " << fitResult.w << " shift: " << fitResult.dw << std::endl;
	//std::cout << "fres1: " << fitResult.fres[0] << " fres2: " << fitResult.fres[1] << " fres3: " << fitResult.fres[2] << " | s1: " << fitResult.sigmares[0]  << " s2: " << fitResult.sigmares[1] << " s3: " << fitResult.sigmares[2]  << std::endl;
	//std::cout << "fbkg: " <<  fitResult.fbkg[0] << " s1: " << fitResult.sigmabkg[0]  << " s2: " << fitResult.sigmabkg[1]  << std::endl;
	return fitResult;
	//BHist->Draw("hesame");
	//BbarHist->Draw("hesame");
}
