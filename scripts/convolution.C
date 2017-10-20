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

	RooRealVar tauB("tauB","B lifetime",1.5,0.1,3) ;
	RooRealVar A("A","A parameter",0.1,-1,1) ;
	RooRealVar S("S","S parameter",0.7,-1,1) ;
	RooRealVar dM("dM","dM parameter",0.51,0,1) ;

	RooRealVar w("w","w parameter",0.2,0,1) ;
	RooRealVar dw("dw","dw parameter",0,-1,1) ;
	RooRealVar fsig("fsig","dw parameter",0,-1,1) ;
	
	RooGenericPdf genpdfB("genpdfB","genpdf","exp(-abs(t)/tauB)/(4*tauB)*( 1 + (A*cos(dM*t)+S*sin(dM*t)) )",RooArgSet(t,tauB, A,S,dM)); 
	RooGenericPdf genpdfBbar("genpdfBbar","genpdf","exp(-abs(t)/tauB)/(4*tauB)*( 1 - (A*cos(dM*t)+S*sin(dM*t)) )",RooArgSet(t,tauB, A,S,dM)); 

	// Construct gauss(t,mg,sg)
	RooRealVar mgSig("mgSig","mg",0) ;
	RooRealVar sgSig("sgSig","sg",0.7,0.1,10) ;
	RooGaussian gaussSig("gaussSig","gauss",t,mgSig,sgSig) ;

	// Construct gauss(t,mg,sg)
	RooRealVar mgBkg("mgBkg","mg2",0) ;
	RooRealVar sgBkg("sgBkg","sg2",0.7,0.1,10) ;
	RooGaussian gaussBkG("gaussBkg","gauss",t,mgBkg,sgBkg) ;

   // C o n s t r u c t   c o n v o l u t i o n   p d f 
   // ---------------------------------------

   // Set #bins to be used for FFT sampling to 10000
	t.setBins(10000,"cache") ; 

   // Construct landau (x) gauss
	RooFFTConvPdf gxgb("gxgb","genpdfB (X) gaussSig",t,genpdfB,gaussSig) ;
	RooFFTConvPdf gxgbbar("gxgbbar","genpdfBbar (X) gaussSig",t,genpdfBbar,gaussSig) ;



   // S a m p l e ,   f i t   a n d   p l o t   c o n v o l u t e d   p d f 
   // ----------------------------------------------------------------------

   // Sample 1000 events in x from gxlx
   RooDataSet* data = gxgb.generate(t,10000) ;

   // Fit gxlx to data
   gxgb.fitTo(*data) ;

   // Plot data, landau pdf, landau (X) gauss pdf
   RooPlot* frame = t.frame(Title("genpdfB (x) gauss convolution")) ;
   data->plotOn(frame) ;
   gxgb.plotOn(frame) ;
   //landau.plotOn(frame,LineStyle(kDashed)) ;


   // Draw frame on canvas
   new TCanvas("rf208_convolution","rf208_convolution",600,600) ;
   gPad->SetLeftMargin(0.15) ; frame->GetYaxis()->SetTitleOffset(1.4) ; frame->Draw() ;

}



