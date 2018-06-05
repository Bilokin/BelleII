void rrange(RooRealVar & mbc, int m, RooRealVar & de, int d, RooRealVar & cs, int c)
// 0: bkg
// 1: signal
// 2: full range
{
	if(m==0){mbc.setRange("Signal",5.2,5.27);} 	//BKG
	if(m==1){mbc.setRange("Signal",5.27,5.3);} 	//SIG
	if(m==2){mbc.setRange("Signal",5.2,5.3);} 	//FULL
	
	if(d==0){de.setRange("Signal",0.1,0.2);} 		//BKG
	if(d==1){de.setRange("Signal",-0.15,0.1);}	//SIG
	if(d==2){de.setRange("Signal",-0.2,0.2);} 	//FULL
	
	if(c==0){cs.setRange("Signal",0.,0.3);} 		//BKG
	if(c==1){cs.setRange("Signal",0.3,1.8);} 		//SIG
	if(c==2){cs.setRange("Signal",0.,2.5);} 		//FULL
	
}


void sigbkgFitResult(RooAbsPdf * combined, RooAbsPdf * bkgpdf, RooAbsPdf * sigpdf, RooDataSet * data, RooRealVar & mbc, RooRealVar & de, RooRealVar & dt, RooRealVar & cs, RooCategory & q)
//(combined,bkgpdf,sigpdf,data,mbc,de,dt,cs,q)
{
	mbc.setBins(50) ;
	cs.setBins(50) ;
	de.setBins(25) ;
	dt.setBins(50) ;
	dt.setRange(-10,10);
	
	TLatex* txtWatermark = new TLatex(0.2,0.87,"BELLE II SIMULATION") ;
	txtWatermark->SetNDC();
	txtWatermark->SetTextSize(0.05) ;
	txtWatermark->SetTextColor(kRed+3) ;
	TLatex* txt = new TLatex(0.2,0.8,"Signal region") ;
	txt->SetNDC();
	txt->SetTextSize(0.05) ;
	txt->SetTextColor(kGray) ;
	
	RooPlot* frame = mbc.frame(Title("M_{bc}")) ;
	frame->SetNdivisions(505,"X");
	RooPlot* deframe = de.frame(Title("#Delta E")) ;
	RooPlot* csframe = cs.frame(Title("CSMVA")) ;
	RooPlot* dtframe = dt.frame(Title("#Delta t"), Range(-10,10)) ;
	frame->SetTitle("");
	deframe->SetTitle("");
	dtframe->SetTitle("");
	csframe->SetTitle("");
	
	dtframe->addObject(txt) ;
	dtframe->addObject(txtWatermark) ;
	frame->addObject(txt) ;
	deframe->addObject(txt) ;
	csframe->addObject(txt) ;
	
	// 0: bkg, 1: signal, 2: full range
	rrange(mbc,2,de,1,cs,1);
	data->plotOn(frame, MarkerStyle(22), CutRange("Signal")) ;
	combined->plotOn(frame,ProjectionRange("Signal")) ;
  combined->plotOn(frame,ProjectionRange("Signal"), Components(*bkgpdf),LineStyle(kDashed), LineColor(kGray)) ;
  
	rrange(mbc,1,de,2,cs,1);
	data->plotOn(deframe, MarkerStyle(22), CutRange("Signal")) ;
	combined->plotOn(deframe,ProjectionRange("Signal")) ;
  combined->plotOn(deframe,ProjectionRange("Signal"), Components(*bkgpdf),LineStyle(kDashed), LineColor(kGray)) ;
  
	rrange(mbc,1,de,1,cs,2);
	data->plotOn(csframe, MarkerStyle(22), CutRange("Signal")) ;
  combined->plotOn(csframe,ProjectionRange("Signal")) ;
  combined->plotOn(csframe,ProjectionRange("Signal"), Components(*bkgpdf),LineStyle(kDashed), LineColor(kGray)) ;
  
	rrange(mbc,1,de,1,cs,1);
	data->plotOn(dtframe, MarkerStyle(22), CutRange("Signal")) ;
  combined->plotOn(dtframe,  ProjectionRange("Signal")) ;
  combined->plotOn(dtframe, ProjectionRange("Signal"), Components(*bkgpdf), LineStyle(kDashed), LineColor(kGray)) ;
  combined->plotOn(dtframe, ProjectionRange("Signal"), Components(*sigpdf), Slice(q,"B0"),LineStyle(kDashed), LineColor(kBlue)) ;
  combined->plotOn(dtframe, ProjectionRange("Signal"), Components(*sigpdf), Slice(q,"B0bar"),LineStyle(kDashed), LineColor(kRed)) ;
        
	
	TCanvas* c = new TCanvas("bphysics","bphysics sig",900,900) ;
	c->Divide(2,2);
	c->cd(1);
	gPad->SetLeftMargin(0.15) ; frame->GetYaxis()->SetTitleOffset(1.6) ; frame->Draw() ;
	c->cd(2);
	gPad->SetLeftMargin(0.15) ; deframe->GetYaxis()->SetTitleOffset(1.6) ; deframe->Draw() ;
	c->cd(3);
	gPad->SetLeftMargin(0.15) ; dtframe->GetYaxis()->SetTitleOffset(1.6) ; dtframe->Draw() ;
	c->cd(4);
	gPad->SetLeftMargin(0.15) ; csframe->GetYaxis()->SetTitleOffset(1.6) ; csframe->Draw() ;
	
	
	//______________________________BKG_________________________________________________________
	
	
	TLatex* txt1 = new TLatex(0.2,0.8,"Bkg region") ;
	txt1->SetNDC();
	txt1->SetTextSize(0.05) ;
	txt1->SetTextColor(kGray) ;
	
	RooPlot* frame2 = mbc.frame(Title("M_{bc}")) ;
	frame2->SetNdivisions(505,"X");
	RooPlot* deframe2 = de.frame(Title("#Delta E")) ;
	RooPlot* csframe2 = cs.frame(Title("CSMVA")) ;
	RooPlot* dtframe2 = dt.frame(Title("#Delta t"), Range(-10,10)) ;
	frame2->SetTitle("");
	deframe2->SetTitle("");
	dtframe2->SetTitle("");
	csframe2->SetTitle("");
	
	dtframe2->addObject(txt1) ;
	dtframe2->addObject(txtWatermark) ;
	frame2->addObject(txt1) ;
	deframe2->addObject(txt1) ;
	csframe2->addObject(txt1) ;
	
	// 0: bkg, 1: signal, 2: full range
	rrange(mbc,2,de,0,cs,0);
	data->plotOn(frame2, MarkerStyle(22), CutRange("Signal")) ;
	combined->plotOn(frame2,ProjectionRange("Signal")) ;
  combined->plotOn(frame2,ProjectionRange("Signal"), Components(*bkgpdf),LineStyle(kDashed), LineColor(kGray)) ;
  
	rrange(mbc,0,de,2,cs,0);
	data->plotOn(deframe2, MarkerStyle(22), CutRange("Signal")) ;
	combined->plotOn(deframe2,ProjectionRange("Signal")) ;
  combined->plotOn(deframe2,ProjectionRange("Signal"), Components(*bkgpdf),LineStyle(kDashed), LineColor(kGray)) ;
  
	rrange(mbc,0,de,0,cs,2);
	data->plotOn(csframe2, MarkerStyle(22), CutRange("Signal")) ;
  combined->plotOn(csframe2,ProjectionRange("Signal")) ;
  combined->plotOn(csframe2,ProjectionRange("Signal"), Components(*bkgpdf),LineStyle(kDashed), LineColor(kGray)) ;
  
	rrange(mbc,0,de,0,cs,0);
	data->plotOn(dtframe2, MarkerStyle(22), CutRange("Signal")) ;
  combined->plotOn(dtframe2,  ProjectionRange("Signal")) ;
  combined->plotOn(dtframe2, ProjectionRange("Signal"), Components(*bkgpdf), LineStyle(kDashed), LineColor(kGray)) ;
  combined->plotOn(dtframe2, ProjectionRange("Signal"), Components(*sigpdf), Slice(q,"B0"),LineStyle(kDashed), LineColor(kBlue)) ;
  combined->plotOn(dtframe2, ProjectionRange("Signal"), Components(*sigpdf), Slice(q,"B0bar"),LineStyle(kDashed), LineColor(kRed)) ;
        
	
	TCanvas* c2 = new TCanvas("bphysic","bphysics bkg",900,900) ;
	c2->Divide(2,2);
	c2->cd(1);
	gPad->SetLeftMargin(0.15) ; frame2->GetYaxis()->SetTitleOffset(1.6) ; frame2->Draw() ;
	c2->cd(2);
	gPad->SetLeftMargin(0.15) ; deframe2->GetYaxis()->SetTitleOffset(1.6) ; deframe2->Draw() ;
	c2->cd(3);
	gPad->SetLeftMargin(0.15) ; dtframe2->GetYaxis()->SetTitleOffset(1.6) ; dtframe2->Draw() ;
	c2->cd(4);
	gPad->SetLeftMargin(0.15) ; csframe2->GetYaxis()->SetTitleOffset(1.6) ; csframe2->Draw() ;
	
	rrange(mbc,1,de,1,cs,1); //for plotAsymmetryResult (all in signal region)
}



