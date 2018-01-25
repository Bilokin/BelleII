#include "fitFunctions.C"
#include "fitSettings.C"
#ifndef	__MYROOFIT__
#define __MYROOFIT__
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
#include "RtypesCore.h"
#include "TCanvas.h"
#include "TAxis.h"
#include "RooPlot.h"
using namespace RooFit ;
#endif

void convolution(fitSettings set, TTree* tree = NULL, bool showSecCanvas = true)
{
	std::cout << " ____________________________" << std::endl;
	std::cout << "|                            |" << std::endl;
	std::cout << "|   FINAL FIT version 0.3    |" << std::endl;
	std::cout << "|____________________________|\n" << std::endl;
	set.Print();
	std::cout << "____________________________\n" << std::endl;
	string name = "MC";
	// Parameters
	RooRealVar dt("dt","#Delta t [ps]",-20,20) ;


	RooCategory q("q","Flavour of the tagged B0") ;
		q.defineType("B0",1) ;
		q.defineType("B0bar",-1) ;
	//RooRealVar w("w","flavour mistag rate",set.w);
	RooRealVar w("w","flavour mistag rate",0,0.5);
	RooRealVar A("A","A",0.1,-1,1);
	RooRealVar S("S","S",0.3,-1,1);

	RooDataSet* data = NULL;
	RooDataSet* wdata = NULL;
	if (tree) 
	{
		std::cout << "Importing tree: ";// << std::endl;
		data = new RooDataSet(name.c_str(), name.c_str(), RooArgSet(dt,q,w), Import(*tree));
		data->Print();
		std::cout << "Done!" << std::endl;
	}
	//RooAbsPdf * combinedQ = getDeltaTCombined(set, dt,q,w, A, S);
	bool fixBkgShape = true;
	RooRealVar* fsig1  = new RooRealVar("fsig1","fsig parameter",set.fsig);

	RooAbsPdf * bcp = getDeltaTSignal(set,dt,q,w,A,S);
	RooAbsPdf * combinedBkg = getDeltaTBkg(set,dt, fixBkgShape);
	
	RooAddPdf * combinedQ = new RooAddPdf("combinedQ","combinedQ", RooArgList(*bcp,*combinedBkg), RooArgList(*fsig1));
	combinedQ->Print();
	//RooAbsPdf * combinedBkg = (RooAbsPdf*)combinedQ->pdfList().at(1);
	// Generate some data
	if (!data) 
	{
		int nevents = 1300;   // 
		//int nevents = 1315.12/set.fsig;   // PHASE III  2 ab^-1 DATASET
		//nevents = 32877.9/set.fsig; // FULL      50 ab^-1 DATASET
		//nevents = 47471; // Total MC DATASET
		w.setRange(0.,0.5);
		RooGenericPdf wpdf("wpdf","0.0275595+-0.350588*w+3.81378*pow(w,2)+-17.1999*pow(w,3)+33.2703*pow(w,4)+-22.3068*pow(w,5)",RooArgList(w));
		std::cout << " _________________________________________ \n" << std::endl;
		std::cout << "       Generating " << nevents << " Toy MC events" << std::endl;
		std::cout << " _________________________________________ " << std::endl;
		wdata = wpdf.generate(RooArgSet(w),nevents);
		data = combinedQ->generate(RooArgSet(dt,q), nevents);
		data->merge(wdata);
		A.setVal(0);
		S.setVal(0);
	}
	RooFitResult* resb = combinedQ->fitTo(*data, Save()) ;
		std::cout << "Here!" << std::endl;
	// Plot B0 and B0bar tagged data separately
	dt.setRange(-10,10);
	RooPlot* frame = dt.frame(Title("B decay (B0/B0bar)")) ;
	
	dt.setBins(25) ;
	data->plotOn(frame,Cut("q==q::B0"),MarkerColor(kBlue), MarkerStyle(22)) ;
	combinedQ->plotOn(frame, Slice(q,"B0"),LineStyle(kDashed), LineColor(kBlue)) ;
	//combinedSignalRes.plotOn(frame, Normalization(1,RooAbsReal::Relative), LineStyle(kDashed), LineColor(kMagenta)) ;
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
	if (showSecCanvas) 
	{
		w.setBins(50) ;
		TCanvas* cw = new TCanvas("cw","w",500,500) ;
		RooPlot* framew = w.frame(Title("w (B0/B0bar)"));
		data->plotOn(framew) ;
		gPad->SetLeftMargin(0.15) ; framew->GetYaxis()->SetTitleOffset(1.6) ; framew->Draw() ;
	}
	std::cout << "-----------------------------" << std::endl;
	std::cout << "-----------------------------" << std::endl;
	std::cout << "-----------------------------" << std::endl;
	resb->Print();
}

void convolution3(float fsig = 0.84, bool showSecCanvas = false)
{
	fitSettings settings;
	//settings.useDeltaResolution = true;
	settings.fsig = fsig;
	settings.dw = -0.005;
	settings.w = 0.27;
	settings.fbkg =     {0.49, 0.51};
	settings.sigmabkg = {4.23,  1.4};
	settings.fres =     {0.35, 0.23, 0.42};
	settings.sigmares = {0.57, 4.65,  1.48};
	settings.wparameters = {0.50072, -0.505257};
	
	convolution(settings, NULL, showSecCanvas);
}

