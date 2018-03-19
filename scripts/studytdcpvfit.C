#include "fitSettings.C"
#include "tdcpvSettings.C"
#include "fitFunctions.C"
#include "RooGlobalFunc.h"
#include "RooRealVar.h"
#include "RooDataSet.h"
#include "RooGaussian.h"
#include "RooConstVar.h"
#include "RooCategory.h"
#include "RooBMixDecay.h"
#include "RooBCPEffDecay.h"
#include "RooBDecay.h"
#include "RooFormulaVar.h"
#include "RooTruthModel.h"
#include "RooGaussModel.h"
#include "RooAddModel.h"
#include "RooAddPdf.h"
#include "RooBCPGenDecay.h"
#include "RooGenericPdf.h"
#include "RooMCStudy.h"
#include "RooPlot.h"
#include "RooEllipse.h"
#include "RooFitResult.h"
#include "TCanvas.h"
#include "TAxis.h"
#include "TH2.h"
#include "TStyle.h"
#include "TMarker.h"
#include "TPaveText.h"
#include "TDirectory.h"
#include "TFile.h"
using namespace RooFit ;

RooRealVar dt("dt","#Delta t [ps]",-10,10) ;
RooCategory q("q","Flavour of the tagged B0") ;
RooRealVar A("A","A",0.,-0.5,0.5);
RooRealVar S("S","S",0.,-0.5,0.5);
RooDataSet* data = NULL;
RooDataSet* wdata = NULL;
RooRealVar Agen("A","A",0.,-0.5,0.5);
RooRealVar Sgen("S","S",0.,-0.5,0.5);
RooRealVar fsig("fsig","signal faction",1.);
RooRealVar dm("dm","delta m(B0)",0.509);
RooRealVar tau("tau","tau (B0)",1.547);
RooRealVar w("w","flavour mistag rate",.2);
RooRealVar dw("dw","delta mistag rate for B0/B0bar",.01);
RooRealVar CPeigen("CPeigen","CP eigen value",1);
RooRealVar effR("effR","B0/B0bar reco efficiency ratio",0.) ;
RooRealVar fres1("fres1","fres parameter",0);
RooRealVar fres2("fres2","fres parameter",0);
RooRealVar fres3("fres3","fres parameter",0);
RooRealVar mgSigB("mgSigB","mg",0);
RooRealVar sgSigB1("sgSigB1","sg",0);
RooRealVar sgSigB2("sgSigB2","sg",0);
RooRealVar sgSigB3("sgSigB3","sg",0);
RooRealVar fbkg1("fbkg1","fbkg parameter",0);
RooRealVar fbkg2("fbkg2","fbkg parameter",0);
RooRealVar mgBkg("mgBkg","mg2",0);
RooRealVar sgBkg1("sgBkg1","sg2",0);
RooRealVar sgBkg2("sgBkg2","sg2",0);
RooRealVar fsigf("fsigf","signal faction",1.);
RooRealVar wf("wf","flavour mistag rate",.2);
RooRealVar dwf("dwf","delta mistag rate for B0/B0bar",.01);
RooGaussModel* gaussSigB1;
RooGaussModel* gaussSigB2;
RooGaussModel* gaussSigB3;
RooAddModel* combinedSignalRes;
RooGaussian* gaussBkg1;
RooGaussian* gaussBkg2;
RooAddPdf* combinedBkg;
RooBCPGenDecay *bcp;
RooAddPdf* combinedQ;
RooGenericPdf* wpdf;
RooBCPGenDecay *bcpf;
RooAddPdf* combinedF;

//-------------------------------------------------------------------------
// Create the PDF models for generation and fit
//  then generate data if no tree provided
//
// Original: Sviatoslav Bilokin, 2017/11/27
// Modifications:
//   2017/12/01 JB
//   2017/12/19 JB Sviatoslav implementation of w profile e-by-e
//
void convolution(tdcpvSettings set, TTree* tree = NULL)
{
  if( set.ifVerbose ) {
    std::cout << " ____________________________" << std::endl;
    std::cout << "|                            |" << std::endl;
    std::cout << "|   FINAL FIT version 0.5    |" << std::endl;
    std::cout << "|____________________________|\n" << std::endl;
    set.Print();
    std::cout << "____________________________\n" << std::endl;
  }
  string name = "MC";
  // Parameters
  //dt.setBins(50) ;
  q.defineType("B0",1) ;
  q.defineType("B0bar",-1) ;
  
  
  
  // Model for generation
  
  // Use delta function resolution model
  //RooTruthModel truthModel("tm","truth model",dt);
  
  sgSigB1.setVal(set.sigmares[0]);
  gaussSigB1 = new RooGaussModel("gaussSigB1","gauss", dt, mgSigB, sgSigB1);
  
  sgSigB2.setVal(set.sigmares[1]);
  gaussSigB2 = new RooGaussModel("gaussSigB2","gauss", dt, mgSigB, sgSigB2);
  
  sgSigB3.setVal(set.sigmares[2]);
  gaussSigB3 = new RooGaussModel("gaussSigB3","gauss", dt, mgSigB, sgSigB3);
  
  fres1.setVal(set.fres[0]);
  fres2.setVal(set.fres[1]);
  fres3.setVal(set.fres[2]);
  combinedSignalRes = new RooAddModel("combinedSignalRes","combinedSignalRes", RooArgList(*gaussSigB1, *gaussSigB2, *gaussSigB3), RooArgList(fres1, fres2, fres3));
  
  sgBkg1.setVal(set.sigmabkg[0]);
  gaussBkg1 = new RooGaussian("gaussBkg1","gauss",dt,mgBkg,sgBkg1);
  
  sgBkg2.setVal(set.sigmabkg[1]);
  gaussBkg2 = new RooGaussian("gaussBkg2","gauss",dt,mgBkg,sgBkg2);
  
  fbkg1.setVal(set.fbkg[0]);
  fbkg2.setVal(set.fbkg[1]);
  combinedBkg = new RooAddPdf("combinedBkg","combinedBkg", RooArgList(*gaussBkg1,*gaussBkg2), RooArgList(fbkg1));
  
  // Construct Bdecay with CP violation
  Agen.setVal(set.Agen);
  Sgen.setVal(set.Sgen);
  fsig.setVal(set.fsig);
  w.setVal(set.w);
  dw.setVal(set.dw);
  //RooBCPEffDecay bcp("bcp","bcp", dt, q, tau, dm, w, CPeigen, absLambda, argLambda, effR, dw, truthModel, RooBCPEffDecay::DoubleSided) ;
  bcp = new RooBCPGenDecay("bcp","bcp", dt, q, tau, dm, w, Agen, Sgen, dw, effR, *combinedSignalRes, RooBCPGenDecay::DoubleSided) ;
  
  combinedQ = new RooAddPdf("combinedQ","combinedQ", RooArgList(*bcp,*combinedBkg), RooArgList(fsig));
	 
  //
  w.setRange(0.,0.5);
	wpdf =	new RooGenericPdf("wpdf","0.0275595+-0.350588*w+3.81378*pow(w,2)+-17.1999*pow(w,3)+33.2703*pow(w,4)+-22.3068*pow(w,5)",RooArgList(w));

  
  // Model for fit
  
  A.setVal(set.Ainit);
  S.setVal(set.Sinit);
  fsigf.setVal(set.fsiginit);
  wf.setVal(set.winit);
  dwf.setVal(set.dwinit);
  bcpf = new RooBCPGenDecay("bcpf","bcpf", dt, q, tau, dm, wf, A, S, dwf, effR, *combinedSignalRes, RooBCPGenDecay::DoubleSided) ;

  combinedF = new RooAddPdf("combinedF","combinedF", RooArgList(*bcpf,*combinedBkg), RooArgList(fsigf));

  
  
  // Generate some data
  if (tree)
  {
    data = new RooDataSet(name.c_str(), name.c_str(), RooArgSet(dt,q), Import(*tree));
  }
  

  if (!data)
  {
    int nevents = 657.6*set.lumi; // lumi in ab-1
    if( set.ifVerbose ) {
      std::cout << " _________________________________________ \n" << std::endl;
      std::cout << "       Generating " << nevents << " Toy MC events" << std::endl;
      std::cout << " _________________________________________ " << std::endl;
    }
    wdata = wpdf->generate(RooArgSet(w),nevents);
    data = combinedQ->generate(RooArgSet(dt,q), ProtoData(*wdata));
  }
  if( set.ifVerbose ) data->get(0)->Print();
  
}

//-------------------------------------------------------------------------
//  Fit the available data (from tree of generated)
//
// Original: Sviatoslav Bilokin, 2017/11/27
// Modifications:
//   2017/11/30 JB
//
void fit(tdcpvSettings set, TTree* tree = NULL)
{
  
  if( set.ifVerbose ) {
    std::cout << " _________________________________________ \n" << std::endl;
    std::cout << "       Fitting " << std::endl;
    std::cout << " _________________________________________ " << std::endl;
  }

  // Do the fit
  RooFitResult* resb = combinedF->fitTo(*data, Save(), PrintLevel(set.ifVerbose-1), Verbose(set.ifVerbose>0) ) ;
  
  if( set.ifDisplay ) {
    cout << "       Displaying " << endl;
    // Plot B0 and B0bar tagged data separately
    RooPlot* frame = dt.frame(Title("B decay (B0/B0bar)")) ;
    
    data->plotOn(frame,Cut("q==q::B0"),MarkerColor(kBlue), MarkerStyle(22)) ;
    combinedQ->plotOn(frame, Slice(q,"B0"),LineStyle(kDashed), LineColor(kBlue)) ;
    
    data->plotOn(frame,Cut("q==q::B0bar"),MarkerColor(kRed), MarkerStyle(23)) ;
    combinedQ->plotOn(frame, Slice(q,"B0bar"),LineStyle(kDashed), LineColor(kRed)) ;
    combinedQ->plotOn(frame, Components(*combinedBkg), Slice(q,"B0bar"),LineStyle(kDashed), LineColor(kGray)) ;
    TCanvas* c = new TCanvas("bphysics","bphysics",1000,500) ;
    c->Divide(2,1);
    c->cd(1);
    gPad->SetLeftMargin(0.15) ; frame->GetYaxis()->SetTitleOffset(1.6) ; frame->Draw() ;
    float Arange[2] = {-0.3, 0.3};
    float Srange[2] = {-0.3, 0.3};
    RooPlot* frame2 = new RooPlot(A,S,Arange[0], Arange[1],Srange[0],Srange[1]) ;
    frame2->SetTitle("Ellipse") ;
    resb->plotOn(frame2,A,S,"MEHV") ;
    c->cd(2);
    gPad->SetLeftMargin(0.15) ; frame2->GetYaxis()->SetTitleOffset(1.4) ; frame2->Draw() ;
  }
  
  if( set.ifVerbose ) {
    std::cout << "-----------------------------" << std::endl;
    std::cout << "-----------------------------" << std::endl;
    std::cout << "-----------------------------" << std::endl;
    //    S.Print();
    //    A.Print();
    resb->Print();
  }
  
}

//-------------------------------------------------------------------------
// Fit generated data
//
void fittdcpv(float lumi = 1 /* ab-1*/, float Agen = 0.0, float Sgen = 0.1, float fsig = 0.7)
{
  
  tdcpvSettings settings;
  settings.lumi = lumi;
  settings.Agen = Agen;
  settings.Sgen = Sgen;
  settings.fsig = fsig;
  settings.dw = 0.01;
  settings.w = 0.2;
  settings.fres = {0.67, 0.05, 0.28};
  settings.fbkg = {0.22, 0.78};
  settings.sigmabkg = {4.3, 1.5};
  settings.sigmares = {0.57, 4.6, 1.46};
  settings.fsiginit = fsig; // to be changed
  settings.dwinit = 0.01;
  settings.winit = 0.2;
  settings.ifDisplay = 1;
  settings.ifVerbose = 1;
  
  convolution(settings, NULL);
  fit(settings, NULL);
  
}

// Same function with full settings as argument
void fittdcpv( tdcpvSettings settings)
{

  convolution(settings, NULL);
  fit(settings, NULL);

}

//-------------------------------------------------------------------------
// Perform a toy MC study based on repetions of same pseudo-experiences
//  to quantify the quality of estimators
//
// Original: Jerome Baudot, 2017/11/30
//
void studytdcpvfit(int nexperiences, tdcpvSettings settings, TString tag="") {
	fitSettings fsettings = getStdSettings();
	bool fixBkgShape = false;
	RooRealVar mbc("mbc","m_{bc} [GeV]",5.2,5.30) ;
	RooRealVar de("de","#Delta E [GeV]",-0.2,0.2) ;
	RooRealVar cs("cs","CSMVA []",0,2) ;

	RooAbsPdf * mbcbkg = getMbcBkg(fsettings, mbc, fixBkgShape);
	RooAbsPdf * debkg = getDeBkg(fsettings, de, fixBkgShape);
	RooAbsPdf * csbkg = getCsBkg(fsettings, cs, fixBkgShape);
	RooAbsPdf * dtbkg = getDeltaTBkg(fsettings, dt, fixBkgShape);

	RooAbsPdf * mbcsignal = getMbcSignal(fsettings, mbc);
	RooAbsPdf * designal = getDeSignal(fsettings, de);
	RooAbsPdf * cssignal = getCsSignal(fsettings, cs);
	RooAbsPdf * dtsignal = getDeltaTSignal(fsettings, dt,q,w,A,S);

	// --- Build Product PDF ---
	RooProdPdf * bkgpdf = new RooProdPdf("bkg","bkg",RooArgList(*mbcbkg, *debkg, *csbkg, *dtbkg));
	RooProdPdf * sigpdf = new RooProdPdf("signal","signal",RooArgSet(*mbcsignal, *designal, *cssignal, *dtsignal));
	//RooProdPdf * bkgpdf = new RooProdPdf("bkg","bkg",RooArgList(*mbcbkg, *debkg,  *dtbkg));
	//RooProdPdf * sigpdf = new RooProdPdf("signal","signal",RooArgSet(*mbcsignal, *designal,  *dtsignal));

	RooAddPdf * combined = new RooAddPdf("combined","combined", RooArgList(*sigpdf, *bkgpdf), fsig);
  
  // create the PDFs
  convolution(settings, NULL);
  
  // manager for the toy study, note the generator and fit models can be different
  RooMCStudy* mcstudy = new RooMCStudy(*combined,RooArgSet(mbc,de,cs,dt,q),FitModel(*combined),Silence()) ;
  
  // generate and fit
  int nevents = 657.6*settings.lumi; // lumi in ab-1
  mcstudy->generateAndFit( nexperiences, nevents) ;
  
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
  outfilename += (int)(settings.Agen*10);
  outfilename += "_genS";
  outfilename += (int)(settings.Sgen*10);
  outfilename += "_lumi";
  outfilename += (int)(settings.lumi*1000);
  outfilename += "fbinv";
  if( tag!="" ) {
    outfilename += "_";
    outfilename += tag;
  }
  outfilename += ".pdf";

  // Draw all plots on a canvas
  gStyle->SetPalette(1) ;
  gStyle->SetOptStat(0) ;
  gStyle->SetTitleSize(0.04,"xyz");
  gStyle->SetTitleOffset(1.1,"y");
  gStyle->SetLabelSize(0.04,"xyz");

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
  pText->AddText( posX, posY-iLine*lineHeight, texte);  iLine++;
  sprintf( texte, "Lumi=%.2f ab-1 (%d events) with A=%.2f, S=%.2f", settings.lumi, nevents, settings.Agen, settings.Sgen);
  pText->AddText( posX, posY-iLine*lineHeight, texte);  iLine++;
  sprintf( texte, "Signal fraction %.3f w=%.3f #Deltaw=%.3f", settings.fsig, settings.w, settings.dw);
  pText->AddText( posX, posY-iLine*lineHeight, texte);  iLine++;
  sprintf( texte, "Signal #Deltat resolution as 3 gaussians with %.1f mean", mgSigB.getValV());
  pText->AddText( posX, posY-iLine*lineHeight, texte);  iLine++;
  sprintf( texte, "   {%.3f %.3f %.3f} sigmas and {%.3f %.3f %.3f} weights", settings.sigmares[0], settings.sigmares[1], settings.sigmares[2], settings.fres[0], settings.fres[1], settings.fres[2]);
  pText->AddText( posX, posY-iLine*lineHeight, texte);  iLine++;
  sprintf( texte, "Bkg #Deltat resolution as 2 gaussians with %.1f mean", mgBkg.getValV());
  pText->AddText( posX, posY-iLine*lineHeight, texte);  iLine++;
  sprintf( texte, "   {%.3f %.3f} sigmas and {%.3f %.3f} weights", settings.sigmabkg[0], settings.sigmabkg[1], settings.fbkg[0], settings.fbkg[1]);
  pText->AddText( posX, posY-iLine*lineHeight, texte);  iLine++;

  sprintf( texte, " ");
  pText->AddText( posX, posY-iLine*lineHeight, texte);  iLine++;
  sprintf( texte, "--------- Fitting ----------");
  pText->AddText( posX, posY-iLine*lineHeight, texte);  iLine++;
  sprintf( texte, "Starting values A=%.3f S=%.3f", settings.Ainit, settings.Sinit);
  pText->AddText( posX, posY-iLine*lineHeight, texte);  iLine++;
  sprintf( texte, "Signal fraction %.3f w=%.3f #Deltaw=%.3f", settings.fsiginit, settings.winit, settings.dwinit);
  pText->AddText( posX, posY-iLine*lineHeight, texte);  iLine++;

  sprintf( texte, " ");
  pText->AddText( posX, posY-iLine*lineHeight, texte);  iLine++;
  sprintf( texte, "----- Estimation from %d pseudo-experiments -----", nexperiences);
  pText->AddText( posX, posY-iLine*lineHeight, texte);  iLine++;
  sprintf( texte, "A: mean=%.3f sigma=%.3f", fitestimators.mean(A), fitestimators.sigma(A));
  pText->AddText( posX, posY-iLine*lineHeight, texte);  iLine++;
  sprintf( texte, "S: mean=%.3f sigma=%.3f", fitestimators.mean(S), fitestimators.sigma(S));
  pText->AddText( posX, posY-iLine*lineHeight, texte);  iLine++;
  sprintf( texte, "Correlation_A_S = %.3f", fitestimators.correlation(A,S));
  pText->AddText( posX, posY-iLine*lineHeight, texte);  iLine++;
  
  pText->Draw();
  c->Print( outfilename+"(" );

  
  // PLot on another canvas, the last data generated and the corresponding fit
  RooPlot* frame = dt.frame(Title("B decay (B0/B0bar)")) ;
  data->plotOn(frame,Cut("q==q::B0"),MarkerColor(kBlue), MarkerStyle(22)) ;
  data->plotOn(frame,Cut("q==q::B0bar"),MarkerColor(kRed), MarkerStyle(23)) ;
  combinedF->plotOn(frame, Slice(q,"B0"),LineStyle(kDashed), LineColor(kBlue)) ;
  combinedF->plotOn(frame, Slice(q,"B0bar"),LineStyle(kDashed), LineColor(kRed)) ;
  combinedQ->plotOn(frame, Components(*combinedBkg), Slice(q,"B0bar"),LineStyle(kDashed), LineColor(kGray)) ;

  float Arange[2] = {-0.3, 0.3};
  float Srange[2] = {-0.3, 0.3};
  RooPlot* frame2 = new RooPlot(A,S,Arange[0], Arange[1],Srange[0],Srange[1]) ;
  frame2->SetTitle("Ellipse") ;
//  mcstudy->fitResult(0)->plotOn(frame2,A,S,"MEHV") ; // does not work, Don't know why!!!

  TCanvas* c1 = new TCanvas("bphysics","One pseudo-experiment",1000,500) ;
  c1->Divide(2,1);
  c1->cd(1);
  gPad->SetLeftMargin(0.15) ; frame->GetYaxis()->SetTitleOffset(1.6) ; frame->Draw() ;
  c1->cd(2);
  gPad->SetLeftMargin(0.15) ; frame2->GetYaxis()->SetTitleOffset(1.4) ; frame2->Draw() ;
  c1->Print( outfilename+")" );

  
  // Make RooMCStudy object available on command line after
  // macro finishes
  gDirectory->Add(mcstudy) ;
  
}

// Wrapper with default arguments
void studytdcpvfit(int nexperiences = 10, float lumi = 1 /* ab-1*/,
                   float Agen = 0.0, float Sgen = 0.0, float fsig = 0.7, TString tag="") {
  
  tdcpvSettings settings;
  settings.lumi = lumi;
  settings.Agen = Agen;
  settings.Sgen = Sgen;
  settings.fsig = fsig;
  settings.dw = 0.01;
  settings.w = 0.4;
  settings.fres = {0.67, 0.05, 0.28};
  settings.fbkg = {0.22, 0.78};
  settings.sigmabkg = {4.3, 1.5};
  settings.sigmares = {0.57, 4.6, 1.46};
  settings.fsiginit = fsig;
  settings.dwinit = 0.01;
  settings.winit = 0.2;
  settings.ifDisplay = 0;
  settings.ifVerbose = 0;
  
  studytdcpvfit( nexperiences, settings, tag);
  
}


