void plotMCstudy(RooMCStudy* mcstudy, RooRealVar &A, RooRealVar &S)
{
	// Make some histograms from the estimators
	RooPlot* frameA     = mcstudy->plotParam(A,Bins(40)) ;
	RooPlot* frameAerr  = mcstudy->plotError(A,Bins(40)) ;
	RooPlot* frameApull = mcstudy->plotPull(A,Bins(40),FitGauss(kTRUE)) ;
	RooPlot* frameS     = mcstudy->plotParam(S,Bins(40)) ;
	RooPlot* frameSerr  = mcstudy->plotError(S,Bins(40)) ;
	RooPlot* frameSpull = mcstudy->plotPull(S,Bins(40),FitGauss(kTRUE)) ;
	RooDataSet fitestimators = mcstudy->fitParDataSet();
	TH2* h2corras = fitestimators.createHistogram(A,S) ;
	h2corras->SetTitle("Correlation; A estimator; S estimator");
	RooEllipse* contour1 = new RooEllipse("contour1", fitestimators.mean(A), fitestimators.mean(S), fitestimators.sigma(A), fitestimators.sigma(S), fitestimators.correlation(A,S), 100 /*points*/);
	RooEllipse* contour2 = new RooEllipse("contour1", fitestimators.mean(A), fitestimators.mean(S), 2*fitestimators.sigma(A), 2*fitestimators.sigma(S), fitestimators.correlation(A,S), 100 /*points*/);
	contour2->SetLineColor(2);
	RooEllipse* contour3 = new RooEllipse("contour1", fitestimators.mean(A), fitestimators.mean(S), 3*fitestimators.sigma(A), 3*fitestimators.sigma(S), fitestimators.correlation(A,S), 100 /*points*/);
	contour3->SetLineColor(4);
	contour3->SetTitle("Contour;A;S");


	// Plot distribution of minimized likelihood
	RooPlot* frameLL = mcstudy->plotNLL(Bins(40)) ;

	// Access some of the saved fit results from individual toys
	//  TH2* corrHist000 = mcstudy->fitResult(0).correlationHist("c000") ;

	// Print canvases on file
	TString outfilename = "tdcpvfit_genA";
	TCanvas* c = new TCanvas("c","Toy MC study",900,900) ;
	c->Divide(3,3) ;
	c->cd(1) ; gPad->SetLeftMargin(0.15) ; frameA->GetYaxis()->SetTitleOffset(1.4) ; frameA->Draw() ;
	c->cd(2) ; gPad->SetLeftMargin(0.15) ; frameAerr->GetYaxis()->SetTitleOffset(1.4) ; frameAerr->Draw() ;
	c->cd(3) ; gPad->SetLeftMargin(0.15) ; frameApull->GetYaxis()->SetTitleOffset(1.4) ; frameApull->Draw() ;
	c->cd(4) ; gPad->SetLeftMargin(0.15) ; frameS->GetYaxis()->SetTitleOffset(1.4) ; frameS->Draw() ;
	c->cd(5) ; gPad->SetLeftMargin(0.15) ; frameSerr->GetYaxis()->SetTitleOffset(1.4) ; frameSerr->Draw() ;
	c->cd(6) ; gPad->SetLeftMargin(0.15) ; frameSpull->GetYaxis()->SetTitleOffset(1.4) ; frameSpull->Draw() ;
	c->cd(7) ; gPad->SetLeftMargin(0.15) ; frameLL->GetYaxis()->SetTitleOffset(1.4) ; frameLL->Draw() ;
	c->cd(8) ; gPad->SetLeftMargin(0.15) ; //gPad->SetGrid(1,1);
	contour3->Draw("") ; contour2->Draw("same") ; contour1->Draw("same") ;
	TMarker *sm = new TMarker( 0., 0., 34) ; sm->Draw();
	//h2corras->GetYaxis()->SetTitleOffset(1.4) ; h2corras->Draw("box") ;

	// Write some explanatory text first
	c->cd(9);
	Float_t interline = 2.;
	Float_t textsize = 0.03;
	Float_t lineHeight = textsize*interline;
	Float_t posX=.1, posY=.95;
	Char_t texte[300];
	Int_t iLine = 0;

	TPaveText *pText = new TPaveText( .01,.01,.99,.99);
	pText->SetBorderSize(0);
	pText->SetFillStyle(0);
	pText->SetTextSize(0.04);
	pText->SetTextAlign(11);

	sprintf( texte, "--------- Generation ----------");
	sprintf( texte, " ");
	pText->AddText( posX, posY-iLine*lineHeight, texte);  iLine++;
	pText->AddText( posX, posY-iLine*lineHeight, texte);  iLine++;
	sprintf( texte, "A: mean=%.3f sigma=%.3f", fitestimators.mean(A), fitestimators.sigma(A));
	pText->AddText( posX, posY-iLine*lineHeight, texte);  iLine++;
	sprintf( texte, "S: mean=%.3f sigma=%.3f", fitestimators.mean(S), fitestimators.sigma(S));
	pText->AddText( posX, posY-iLine*lineHeight, texte);  iLine++;
	sprintf( texte, "Correlation_A_S = %.3f", fitestimators.correlation(A,S));
	pText->AddText( posX, posY-iLine*lineHeight, texte);  iLine++;

	pText->Draw();

}
