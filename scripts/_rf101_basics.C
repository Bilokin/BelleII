/// \file
/// \ingroup tutorial_roofit
/// \notebook -js
///  'BASIC FUNCTIONALITY' RooFit tutorial macro #101
///
///  Fitting, plotting, toy data generation on one-dimensional p.d.f
///
///  pdf = gauss(x,m,s)
///
/// \macro_image
/// \macro_output
/// \macro_code
/// \author 07/2008 - Wouter Verkerke 


#include "RooRealVar.h"
#include "RooDataSet.h"
#include "RooGaussian.h"
#include "TCanvas.h"
#include "RooPlot.h"
#include "TAxis.h"
using namespace RooFit ;


void _rf101_basics()
{
   // S e t u p   m o d e l 
   // ---------------------

   // Declare variables x,mean,sigma with associated name, title, initial value and allowed range
   RooRealVar x("x","x",-10,10) ;
   RooRealVar mean("mean","mean of gaussian",1,-10,10) ;
   RooRealVar sigma("sigma","width of gaussian",1,0.1,10) ;

	RooRealVar mbc("mbc","m_{bc} [GeV]",5.20,5.30) ;
   // Build gaussian p.d.f in terms of x,mean and sigma
   RooGaussian gauss("gauss","gaussian PDF",x,mean,sigma) ;  
	RooRealVar sigmean("sigmean","B^{#pm} mass",5.2795, 5.2, 5.3) ;
	RooRealVar sigsigma("sigsigma","B^{#pm} width",6.12e-3,0.000001,1.) ;
	RooRealVar sigalpha("sigalpha","B^{#pm} width",.5, 1. ,4.) ;
	RooRealVar sign("sign","B^{#pm} width",90.8, 1,200.) ;
	RooCBShape CBall("CBall", "Crystal Ball shape", mbc, sigmean, sigsigma, sigalpha, sign);

   // Construct plot frame in 'x'
   RooPlot* xframe = mbc.frame(Title("Gaussian p.d.f.")) ;


   // P l o t   m o d e l   a n d   c h a n g e   p a r a m e t e r   v a l u e s
   // ---------------------------------------------------------------------------

   // Plot gauss in frame (i.e. in x) 
   CBall.plotOn(xframe) ;

   // Change the value of sigma to 3
   sigma.setVal(3) ;

   // Plot gauss in frame (i.e. in x) and draw frame on canvas
  

   // G e n e r a t e   e v e n t s 
   // -----------------------------

   // Generate a dataset of 1000 events in x from gauss
   RooDataSet* data = CBall.generate(mbc,10000) ;  
  
   // Make a second plot frame in x and draw both the 
   // data and the p.d.f in the frame
   RooPlot* xframe2 = mbc.frame(Title("Gaussian p.d.f. with data")) ;
   data->plotOn(xframe2) ;
   CBall.plotOn(xframe2) ;
  

   // F i t   m o d e l   t o   d a t a
   // -----------------------------

   // Fit pdf to data
   CBall.fitTo(*data) ;

   // Print values of mean and sigma (that now reflect fitted values and errors)
   sigmean.Print() ;
   sigsigma.Print() ;

   // Draw all frames on a canvas
   TCanvas* c = new TCanvas("rf101_basics","rf101_basics",800,400) ;
   c->Divide(2) ;
   c->cd(1) ; gPad->SetLeftMargin(0.15) ; xframe->GetYaxis()->SetTitleOffset(1.6) ; xframe->Draw() ;
   c->cd(2) ; gPad->SetLeftMargin(0.15) ; xframe2->GetYaxis()->SetTitleOffset(1.6) ; xframe2->Draw() ;
  
 
}
