#include "fitSettings.C"
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

void convolution(fitSettings set, TTree* tree = NULL, bool showSecCanvas = false)
{
	std::cout << " ____________________________" << std::endl;
	std::cout << "|                            |" << std::endl;
	std::cout << "|   FINAL FIT version 0.3    |" << std::endl;
	std::cout << "|____________________________|\n" << std::endl;
	set.Print();
	std::cout << "____________________________\n" << std::endl;
	string name = "MC";
	// Parameters
	RooRealVar dt("dt","#Delta t [ps]",-10,10) ;
	//dt.setBins(50) ;


	RooCategory q("q","Flavour of the tagged B0") ;
		q.defineType("B0",1) ;
		q.defineType("B0bar",-1) ;
	//RooRealVar w("w","flavour mistag rate",set.w);
	RooRealVar w("w","flavour mistag rate",0,0.5);

	RooDataSet* data = NULL;
	RooDataSet* wdata = NULL;
	if (tree) 
	{
		data = new RooDataSet(name.c_str(), name.c_str(), RooArgSet(dt,q,w), Import(*tree));
	}
	
	RooRealVar fsig1("fsig1","fsig parameter",set.fsig);
	RooRealVar fsig2("fsig2","fsig parameter",1-set.fsig);
	RooRealVar dm("dm","delta m(B0)",0.509);
	RooRealVar tau("tau","tau (B0)",1.547);
	RooRealVar dw("dw","delta mistag rate for B0/B0bar",set.dw);
	// Use delta function resolution model
	//RooTruthModel truthModel("tm","truth model",dt);

	// Additional parameters needed for B decay with CPV
	RooRealVar CPeigen("CPeigen","CP eigen value",1);
	RooRealVar A("A","A",0.1,-1,1);
	RooRealVar S("S","S",0.3,-1,1);
	RooRealVar effR("effR","B0/B0bar reco efficiency ratio",0.) ;

	RooRealVar fres1("fres1","fres parameter",set.fres[0]);
	RooRealVar fres2("fres2","fres parameter",set.fres[1]);
	RooRealVar fres3("fres3","fres parameter",set.fres[2]);

	RooRealVar mgSigB("mgSigB","mg",0);

	RooRealVar sgSigB1("sgSigB1","sg",set.sigmares[0]);
	RooRealVar sgSigB2("sgSigB2","sg",set.sigmares[1]);
	RooRealVar sgSigB3("sgSigB3","sg",set.sigmares[2]);
	RooGaussModel gaussSigB1("gaussSigB1","gauss", dt, mgSigB, sgSigB1);
	RooGaussModel gaussSigB2("gaussSigB2","gauss", dt, mgSigB, sgSigB2);
	RooGaussModel gaussSigB3("gaussSigB3","gauss", dt, mgSigB, sgSigB3);
	
	RooAddModel combinedSignalRes("combinedSignalRes","combinedSignalRes", RooArgList(gaussSigB1,gaussSigB2, gaussSigB3), RooArgList(fres1, fres2,fres3));
	//combinedSignalRes.fixCoefNormalization(RooArgList(fres1, fres2,fres3));
	
	RooRealVar fbkg1("fbkg1","fbkg parameter",set.fbkg[0]);
	RooRealVar fbkg2("fbkg2","fbkg parameter",set.fbkg[1]);
	RooRealVar mgBkg("mgBkg","mg2",0);
	RooRealVar sgBkg1("sgBkg1","sg2",set.sigmabkg[0]);
	RooRealVar sgBkg2("sgBkg2","sg2",set.sigmabkg[1]);
	RooGaussian gaussBkg1("gaussBkg1","gauss",dt,mgBkg,sgBkg1);
	RooGaussian gaussBkg2("gaussBkg2","gauss",dt,mgBkg,sgBkg2);
	
	RooAddPdf combinedBkg("combinedBkg","combinedBkg", RooArgList(gaussBkg2,gaussBkg1), RooArgList(fbkg2, fbkg1));

	// Construct Bdecay with CP violation
	//RooBCPEffDecay bcp("bcp","bcp", dt, q, tau, dm, w, CPeigen, absLambda, argLambda, effR, dw, truthModel, RooBCPEffDecay::DoubleSided) ;
	RooBCPGenDecay bcp("bcp","bcp", dt, q, tau, dm, w, A, S, dw, effR, combinedSignalRes, RooBCPGenDecay::DoubleSided) ;
	
	RooAddPdf combinedQ("combinedQ","combinedQ", RooArgList(bcp,combinedBkg), RooArgList(fsig1));
	 
	// Generate some data
	if (!data) 
	{
		int nevents = 1315.12/set.fsig;   // PHASE III  2 ab^-1 DATASET
		//nevents = 32877.9/set.fsig; // FULL      50 ab^-1 DATASET
		//nevents = 20725/set.fsig; // Total MC DATASET
		w.setRange(0.,0.39);
		RooGenericPdf wpdf("wpdf","0.0161275-0.229947*w+3.25998*pow(w,2)-18.8331*pow(w,3)+45.926*pow(w,4)-37.8687*pow(w,5)",RooArgList(w));
		std::cout << " _________________________________________ \n" << std::endl;
		std::cout << "       Generating " << nevents << " Toy MC events" << std::endl;
		std::cout << " _________________________________________ " << std::endl;
		wdata = wpdf.generate(RooArgSet(w),nevents);
		//data = combinedQ.generate(RooArgSet(dt,q), nevents);
		data = combinedQ.generate(RooArgSet(dt,q), ProtoData(*wdata));
		A.setVal(0);
		S.setVal(0);
	}
	RooFitResult* resb = combinedQ.fitTo(*data, Save()) ;
	// Plot B0 and B0bar tagged data separately
	RooPlot* frame = dt.frame(Title("B decay (B0/B0bar)")) ;

	data->plotOn(frame,Cut("q==q::B0"),MarkerColor(kBlue), MarkerStyle(22)) ;
	combinedQ.plotOn(frame, Slice(q,"B0"),LineStyle(kDashed), LineColor(kBlue)) ;
	//combinedSignalRes.plotOn(frame, Normalization(1,RooAbsReal::Relative), LineStyle(kDashed), LineColor(kMagenta)) ;
	data->plotOn(frame,Cut("q==q::B0bar"),MarkerColor(kRed), MarkerStyle(23)) ;
	combinedQ.plotOn(frame, Slice(q,"B0bar"),LineStyle(kDashed), LineColor(kRed)) ;
	combinedQ.plotOn(frame, Components(combinedBkg), Slice(q,"B0bar"),LineStyle(kDashed), LineColor(kGray)) ;
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

void convolution3(float fsig = 0.7, bool showSecCanvas = false)
{
	fitSettings settings;
	settings.fsig = fsig;
	settings.dw = 0.01;
	settings.w = 0.26;
	settings.fres = {0.67, 0.05, 0.28};
	settings.fbkg = {0.22, 0.78};
	settings.sigmabkg = {4.3, 1.5};
	settings.wparameters = {0.501543, 0.316146};
	settings.sigmares = {0.57, 4.6, 1.46};
	
	convolution(settings, NULL, showSecCanvas);
}

