void plotFitResult(RooAbsPdf * combined, RooAbsPdf * bkgpdf, RooAbsPdf * sigpdf, RooDataSet * data, RooRealVar & mbc, RooRealVar & de, RooRealVar & dt, RooRealVar & cs, RooCategory & q)
{
	mbc.setBins(50) ;
	cs.setBins(50) ;
	de.setBins(25) ;
	dt.setBins(50) ;
	
	RooPlot* frame = mbc.frame(Title("M_{bc}")) ;
	frame->SetNdivisions(505,"X");
	RooPlot* deframe = de.frame(Title("#Delta E")) ;
	RooPlot* csframe = cs.frame(Title("CSMVA")) ;
	RooPlot* dtframe = dt.frame(Title("#Delta t"), Range(-10,10)) ;
	frame->SetTitle("");
	deframe->SetTitle("");
	dtframe->SetTitle("");
	csframe->SetTitle("");
	data->plotOn(frame, MarkerStyle(22)) ;
	data->plotOn(deframe, MarkerStyle(22)) ;
	data->plotOn(csframe, MarkerStyle(22)) ;
        combined->plotOn(frame) ;
        combined->plotOn(frame, Components(*bkgpdf),LineStyle(kDashed), LineColor(kGray)) ;
        combined->plotOn(deframe) ;
        combined->plotOn(deframe, Components(*bkgpdf),LineStyle(kDashed), LineColor(kGray)) ;
        combined->plotOn(csframe) ;
        combined->plotOn(csframe, Components(*bkgpdf),LineStyle(kDashed), LineColor(kGray)) ;
	
	mbc.setRange("Signal",5.27,5.3);
	de.setRange("Signal",-0.15,0.1);
	//mbc.setRange("Signal",5.2,5.3);
	//de.setRange("Signal",-0.2,0.2);
	//cs.setRange("Signal",0.5,2);
	dt.setRange(-10,10);
	TLatex* txtWatermark = new TLatex(0.2,0.87,"BELLE II SIMULATION") ;
	txtWatermark->SetNDC();
	txtWatermark->SetTextSize(0.05) ;
	txtWatermark->SetTextColor(kRed+3) ;
	TLatex* txt = new TLatex(0.2,0.8,"Signal region") ;
	txt->SetNDC();
	txt->SetTextSize(0.05) ;
	txt->SetTextColor(kGray) ;
	dtframe->addObject(txt) ;
	//dtframe->SetMaximum(75) ;
	//frame->SetMaximum(160) ;
	//frame->addObject(txtWatermark) ;
	//deframe->addObject(txtWatermark) ;
	//csframe->addObject(txtWatermark) ;
	dtframe->addObject(txtWatermark) ;
	data->plotOn(dtframe, MarkerStyle(22), CutRange("Signal")) ;
        combined->plotOn(dtframe,  ProjectionRange("Signal")) ;
        combined->plotOn(dtframe, ProjectionRange("Signal"), Components(*bkgpdf), LineStyle(kDashed), LineColor(kGray)) ;
        combined->plotOn(dtframe, ProjectionRange("Signal"), Components(*sigpdf), Slice(q,"B0"),LineStyle(kDashed), LineColor(kBlue)) ;
        combined->plotOn(dtframe, ProjectionRange("Signal"), Components(*sigpdf), Slice(q,"B0bar"),LineStyle(kDashed), LineColor(kRed)) ;

	
	TCanvas* c = new TCanvas("bphysics","bphysics",1000,1000) ;
	c->Divide(2,2);
	c->cd(1);
	gPad->SetLeftMargin(0.15) ; frame->GetYaxis()->SetTitleOffset(1.6) ; frame->Draw() ;
	c->cd(2);
	gPad->SetLeftMargin(0.15) ; deframe->GetYaxis()->SetTitleOffset(1.6) ; deframe->Draw() ;
	c->cd(3);
	gPad->SetLeftMargin(0.15) ; dtframe->GetYaxis()->SetTitleOffset(1.6) ; dtframe->Draw() ;
	c->cd(4);
	gPad->SetLeftMargin(0.15) ; csframe->GetYaxis()->SetTitleOffset(1.6) ; csframe->Draw() ;
}
void plotAsymmetryResult(RooAbsPdf * combined, RooAbsPdf * bkgpdf, RooAbsPdf * sigpdf, RooDataSet * data, RooFitResult* resb, RooRealVar & A, RooRealVar & S, RooRealVar & dt, RooCategory & q)
{
	RooBinning abins(-10,10) ;
	// Add boundaries at 0, (-1,1), (-2,2), (-3,3), (-4,4) and (-6,6)
	abins.addBoundary(0) ;
	abins.addBoundaryPair(1) ;
	abins.addBoundaryPair(2) ;
	abins.addBoundaryPair(3) ;
	abins.addBoundaryPair(4) ;
	abins.addBoundaryPair(6) ;
	float Arange[2] = {-0.3, 0.3};
	float Srange[2] = {-0.3, 0.3};
	RooPlot* frameres = new RooPlot(A,S,Arange[0], Arange[1],Srange[0],Srange[1]) ;
	frameres->SetTitle("");
	resb->plotOn(frameres,A,S,"MEHV") ;
	RooPlot* aframe = dt.frame(Title("mixState asymmetry distribution with custom binning")) ;
	data->plotOn(aframe,Asymmetry(q),Binning(abins), CutRange("Signal")) ;
	combined->plotOn(aframe,Asymmetry(q), ProjectionRange("Signal"), Components(*sigpdf)) ;
	
	TCanvas* c2 = new TCanvas("bphysics2","bphysics",800,400) ;
	c2->Divide(2,1);
	c2->cd(1);
	gPad->SetLeftMargin(0.15) ; frameres->GetYaxis()->SetTitleOffset(1.6) ; frameres->Draw() ;
	c2->cd(2);
	gPad->SetLeftMargin(0.15) ; frameres->GetYaxis()->SetTitleOffset(1.6) ; aframe->Draw() ;
}
