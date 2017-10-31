/// \file
/// \ingroup tutorial_roofit
/// \notebook -js
///  'ADDITION AND CONVOLUTION' RooFit tutorial macro #208
///
///  One-dimensional numeric convolution
///  (require ROOT to be compiled with --enable-fftw3)
///
///  pdf = landau(t) (x) gauss(t)
///
/// \macro_image
/// \macro_output
/// \macro_code
/// \author 07/2008 - Wouter Verkerke 


#include "RooRealVar.h"
#include "RooDataSet.h"
#include "RooGaussian.h"
#include "RooLandau.h"
#include "RooFFTConvPdf.h"
#include "RooPlot.h"
#include "TCanvas.h"
#include "TAxis.h"
#include "TH1.h"
using namespace RooFit ;



void convolution()
{
   // S e t u p   c o m p o n e n t   p d f s 
   // ---------------------------------------

   // Construct observable
	RooRealVar t("t","#Delta t",-10,10) ;

	RooRealVar tauB("tauB","B lifetime",1.52);
	RooRealVar AB("AB","A parameter",0.1,0,1);
	RooRealVar SB("SB","S parameter",0.5,0,1);
	RooRealVar ABbar("ABbar","A parameter",0.1,0,1);
	RooRealVar SBbar("SBbar","S parameter",0.5,0,1);
	RooRealVar dM("dM","dM parameter",0.51);

	RooRealVar wpar("wpar","w parameter",0.2);
	RooRealVar dwpar("dwpar","dw parameter",0.2);
	RooRealVar fsig("fsig","fsig parameter",0.8,0,1);
	
	RooGenericPdf genpdfB("genpdfB","genpdf",      "exp(-abs(t)/tauB)/(4*tauB)*(1-dwpar+(1-2*wpar)*(AB*cos(dM*t)+SB*sin(dM*t)))",RooArgSet(t,tauB,AB,SB,dM,wpar,dwpar)); 
	RooGenericPdf genpdfBbar("genpdfBbar","genpdf","exp(-abs(t)/tauB)/(4*tauB)*(1+dwpar-(1-2*wpar)*(ABbar*cos(dM*t)+SBbar*sin(dM*t)))",RooArgSet(t,tauB,ABbar,SBbar,dM,wpar,dwpar)); 

	// Construct gauss(t,mg,sg)
	RooRealVar mgSigB("mgSigB","mg",0);
	RooRealVar sgSigB("sgSigB","sg",0.7,0.,10);
	RooGaussian gaussSigB("gaussSigB","gauss",t,mgSigB,sgSigB);

	RooRealVar mgSigBbar("mgSigBbar","mg",0);
	RooRealVar sgSigBbar("sgSigBbar","sg",0.7,0.,10);
	RooGaussian gaussSigBbar("gaussSigBbar","gauss",t,mgSigBbar,sgSigBbar);
	
	// Construct gauss(t,mg,sg)
	RooRealVar mgBkg("mgBkg","mg2",0);
	RooRealVar sgBkg("sgBkg","sg2",0.7,0.1,10);
	RooGaussian gaussBkgB("gaussBkg","gauss",t,mgBkg,sgBkg);

	RooRealVar mgBkgbar("mgBkgbar","mg2",0);
	RooRealVar sgBkgbar("sgBkgbar","sg2",0.7,0.1,10);
	RooGaussian gaussBkgBbar("gaussBkgBbar","gauss",t,mgBkgbar,sgBkgbar);

   // C o n s t r u c t   c o n v o l u t i o n   p d f 
   // ---------------------------------------

   // Set #bins to be used for FFT sampling to 10000
	t.setBins(10000,"cache") ; 

   // Construct landau (x) gauss
	RooFFTConvPdf gxgb("gxgb","genpdfB (X) gaussSig",t,genpdfB,gaussSigB);
	RooFFTConvPdf gxgbbar("gxgbbar","genpdfBbar (X) gaussSig",t,genpdfBbar,gaussSigBbar);

	RooAddPdf combinedB("combinedB","combinedB", RooArgList(gxgb,gaussBkgB), fsig);
	RooAddPdf combinedBbar("combinedBbar","combinedBbar", RooArgList(gxgbbar,gaussBkgBbar), fsig);

   // S a m p l e ,   f i t   a n d   p l o t   c o n v o l u t e d   p d f 
   // ----------------------------------------------------------------------

   // Sample 1000 events in x from gxlx
	RooDataSet* data = combinedB.generate(t,10000) ;
	RooDataSet* databar = combinedBbar.generate(t,10000) ;

	// Fit gxlx to data
	combinedB.fitTo(*data) ;
	combinedBbar.fitTo(*databar) ;

	// Plot data, landau pdf, landau (X) gauss pdf
	RooPlot* frame = t.frame(Title("genpdfB (x) gauss convolution")) ;
	data->plotOn(frame, MarkerStyle(22), MarkerColor(kRed)) ;
	databar->plotOn(frame, MarkerStyle(23), MarkerColor(kBlue)) ;
	combinedB.plotOn(frame, LineStyle(kDashed), LineColor(kRed)) ;
	combinedBbar.plotOn(frame, LineStyle(kDashed)) ;
	//landau.plotOn(frame,LineStyle(kDashed)) ;


	// Draw frame on canvas
	new TCanvas("rf208_convolution","rf208_convolution",600,600) ;
	gPad->SetLeftMargin(0.15) ; frame->GetYaxis()->SetTitleOffset(1.4) ; frame->Draw() ;
	std::cout << "B S: " << SB.getValV() << "+-" << SB.getError() << std::endl;
	std::cout << "Bbar S: " << SBbar.getValV() << "+-" << SBbar.getError() << std::endl;

}

