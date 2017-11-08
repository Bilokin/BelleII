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

RooDataSet* createRooData(TTree* tree, string name, string cuts)
{
	RooRealVar t("B0_DeltaT","#Delta t",-10,10);
	//RooDataSet* data = new RooDataSet(name.c_str(), name.c_str(), tree, RooArgSet(t), RooFormulaVar("cut","cut",(cuts).c_str()));
	RooDataSet* data = new RooDataSet(name.c_str(), name.c_str(), tree, RooArgSet(t), (cuts).c_str());
	
	return data;
}

RooDataHist* createRooHist(TH1* hist)
{
	RooRealVar t("t","#Delta t",-10,10);
	RooDataHist * bdata = new RooDataHist("bdata","bdata",RooArgList(t),hist);
	return bdata;
}

void convolution(RooAbsData* data = NULL, RooAbsData* databar = NULL, float wnorm = 0.2, float dwnorm = 0.2, float fsignorm = 0.7)
{
   // S e t u p   c o m p o n e n t   p d f s 
   // ---------------------------------------
	//RooDataSet* data = NULL;
	//RooDataSet* databar = NULL;
	RooDataSet* dataq = NULL;
   // Construct observable
	RooRealVar t("t","#Delta t",-10,10);
	RooCategory q("q","B0 flavor") ;
	q.defineType("Bbar",-1) ;
	q.defineType("B",1) ; 

	RooRealVar tauB("tauB","B lifetime",1.52);
	RooRealVar A("A","A parameter",0.1,0,1);
	RooRealVar S("S","S parameter",0.5,0,1);
	RooRealVar dM("dM","dM parameter",0.51);

	RooRealVar wpar("wpar","w parameter", wnorm);
	RooRealVar dwpar("dwpar","dw parameter", dwnorm);
	RooRealVar fsig("fsig","fsig parameter",fsignorm);
	
	RooGenericPdf genpdfB("genpdfB","genpdf",      "exp(-abs(t)/tauB)/(4*tauB)*(1-dwpar+(1-2*wpar)*(A*cos(dM*t)+S*sin(dM*t)))",RooArgSet(t,tauB,A,S,dM,wpar,dwpar)); 
	RooGenericPdf genpdfBbar("genpdfBbar","genpdf","exp(-abs(t)/tauB)/(4*tauB)*(1+dwpar-(1-2*wpar)*(A*cos(dM*t)+S*sin(dM*t)))",RooArgSet(t,tauB,A,S,dM,wpar,dwpar)); 
	
	RooGenericPdf genpdfQ("genpdfQ","genpdfq","exp(-abs(t)/tauB)/(4*tauB)*(1-dwpar*q+(1-2*wpar)*q*(A*cos(dM*t)+S*sin(dM*t)))",RooArgSet(t,tauB,A,S,dM,wpar,dwpar, q)); 

	// Construct gauss(t,mg,sg)
	RooRealVar fres("fres","fres parameter",0.94);
	RooRealVar mgSigB("mgSigB","mg",0);
	RooRealVar sgSigB1("sgSigB1","sg",0.6);
	RooGaussian gaussSigB1("gaussSigB1","gauss", t, mgSigB, sgSigB1);

	RooRealVar sgSigB2("sgSigB2","sg",4.7);
	RooGaussian gaussSigB2("gaussSigB2","gauss", t, mgSigB, sgSigB2);
	
	// Construct gauss(t,mg,sg)
	RooRealVar fbkg("fbkg","fbkg parameter",0.8);
	RooRealVar mgBkg("mgBkg","mg2",0);
	RooRealVar sgBkg1("sgBkg1","sg2",2);
	RooGaussian gaussBkg1("gaussBkg1","gauss",t,mgBkg,sgBkg1);

	RooRealVar sgBkg2("sgBkg2","sg2",4.4);
	RooGaussian gaussBkg2("gaussBkg2","gauss",t,mgBkg,sgBkg2);

	RooAddPdf combinedSignalRes("combinedSignalRes","combinedSignalRes", RooArgList(gaussSigB1,gaussSigB2), fres);
	RooAddPdf combinedBkg("combinedBkg","combinedBkg", RooArgList(gaussBkg1,gaussBkg2), fbkg);

   // C o n s t r u c t   c o n v o l u t i o n   p d f 
   // ---------------------------------------

   // Set #bins to be used for FFT sampling to 10000
	t.setBins(100,"cache") ; 

   // Construct landau (x) gauss
	RooFFTConvPdf gxgb("gxgb","genpdfB (X) combinedSignalRes",t,genpdfB, combinedSignalRes);
	RooFFTConvPdf gxgbbar("gxgbbar","genpdfBbar (X) combinedSignalRes",t,genpdfBbar, combinedSignalRes);
	//RooFFTConvPdf signalq("signalq","genpdfQr (X) combinedSignalRes",RooArgSet(t,q),genpdfQ, combinedSignalRes);
	RooFFTConvPdf signalq("signalq","genpdfQr (X) combinedSignalRes",t,genpdfQ, combinedSignalRes);

	RooAddPdf combinedB("combinedB","combinedB", RooArgList(gxgb,combinedBkg), fsig);
	RooAddPdf combinedBbar("combinedBbar","combinedBbar", RooArgList(gxgbbar,combinedBkg), fsig);

	RooAddPdf combinedQ("combinedQ","combinedQ", RooArgList(signalq,combinedBkg), fsig);

   // S a m p l e ,   f i t   a n d   p l o t   c o n v o l u t e d   p d f 
   // ----------------------------------------------------------------------

   // Sample 1000 events in x from gxlx
	if (data == NULL || databar == NULL) 
	{
		float nevents = 10000;
		data = combinedB.generate(t,nevents) ;
		databar = combinedBbar.generate(t,nevents);
		dataq = combinedQ.generate(RooArgSet(t,q),nevents*2);
	}
		
	// Fit gxlx to data
	RooFitResult* resb = combinedB.fitTo(*data, Save()) ;
	RooFitResult* resbbar = combinedBbar.fitTo(*databar, Save()) ;

	RooFitResult* resq = combinedQ.fitTo(*dataq, Save()) ;

	// Plot data, landau pdf, landau (X) gauss pdf
	RooPlot* frame = t.frame(Title("genpdfB (x) gauss convolution")) ;
	data->plotOn(frame, MarkerStyle(22), MarkerColor(kRed)) ;
	//dataq->plotOn(frame, Cut("q==q::Bbar"), MarkerColor(kRed+1)) ;
	databar->plotOn(frame, MarkerStyle(23), MarkerColor(kBlue)) ;
	//dataq->plotOn(frame, Cut("q==q::B"), MarkerColor(kBlue+1)) ;
	//combinedB.plotOn(frame, LineStyle(kDashed), LineColor(kRed)) ;
	//combinedBbar.plotOn(frame, LineStyle(kDashed)) ;
	//landau.plotOn(frame,LineStyle(kDashed)) ;


	// Draw frame on canvas
	new TCanvas("rf208_convolution","rf208_convolution",600,600) ;
	gPad->SetLeftMargin(0.15) ; frame->GetYaxis()->SetTitleOffset(1.4) ; frame->Draw() ;

	RooPlot* frame2 = new RooPlot(A,S,0.,1,0.,1) ;
	frame2->SetTitle("Ellipse") ;
	resb->plotOn(frame2,A,S,"ME12ABHV") ;
	new TCanvas("convolution","convolution",500,500);
	gPad->SetLeftMargin(0.15) ; frame2->GetYaxis()->SetTitleOffset(1.4) ; frame2->Draw() ;

	std::cout << "-----------------------------" << std::endl;
	std::cout << "-----------------------------" << std::endl;
	std::cout << "-----------------------------" << std::endl;
	resb->Print();
	std::cout << "-----------------------------" << std::endl;
	resbbar->Print();
	dataq->get(0)->Print();
}

