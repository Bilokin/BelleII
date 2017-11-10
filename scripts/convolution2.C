

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
#include "TCanvas.h"
#include "TAxis.h"
#include "RooPlot.h"
using namespace RooFit ;

void convolution2(TTree* tree = NULL, float fsignorm = 0.3, float wnorm = 0.21, float dwnorm = 0.)
{
	
	std::cout << " ____________________________" << std::endl;
	std::cout << "|                            |" << std::endl;
	std::cout << "|   FINAL FIT version 0.2    |" << std::endl;
	std::cout << "|____________________________|\n" << std::endl;

	string name = "MC";
	// Parameters
	RooRealVar dt("dt","#Delta t [ps]",-10,10) ;
	dt.setBins(50) ;


	RooCategory q("q","Flavour of the tagged B0") ;
		q.defineType("B0",1) ;
		q.defineType("B0bar",-1) ;

	RooDataSet* data = NULL;
	if (tree) 
	{
		data = new RooDataSet(name.c_str(), name.c_str(), RooArgSet(dt,q), Import(*tree));
	}
	
	RooRealVar fsig1("fsig1","fsig parameter",fsignorm);
	RooRealVar fsig2("fsig2","fsig parameter",1-fsignorm);
	RooRealVar dm("dm","delta m(B0)",0.509) ;
	RooRealVar tau("tau","tau (B0)",1.547) ;
	RooRealVar w("w","flavour mistag rate",wnorm) ;
	RooRealVar dw("dw","delta mistag rate for B0/B0bar",dwnorm) ;
	// Use delta function resolution model
	RooTruthModel truthModel("tm","truth model",dt) ;

	// Additional parameters needed for B decay with CPV
	RooRealVar CPeigen("CPeigen","CP eigen value",1) ;
	RooRealVar A("A","A",0.1,0,0.5);
	RooRealVar S("S","S",0.3,0,0.5);
	RooRealVar effR("effR","B0/B0bar reco efficiency ratio",0.) ;

	RooRealVar fres1("fres1","fres parameter",0.67);
	RooRealVar fres2("fres2","fres parameter",0.05);
	RooRealVar fres3("fres3","fres parameter",0.28);
	RooRealVar mgSigB("mgSigB","mg",0);
	RooRealVar sgSigB1("sgSigB1","sg",0.57);
	RooGaussModel gaussSigB1("gaussSigB1","gauss", dt, mgSigB, sgSigB1);

	RooRealVar sgSigB2("sgSigB2","sg",4.6);
	RooGaussModel gaussSigB2("gaussSigB2","gauss", dt, mgSigB, sgSigB2);
	
	RooRealVar sgSigB3("sgSigB3","sg",1.46);
	RooGaussModel gaussSigB3("gaussSigB3","gauss", dt, mgSigB, sgSigB2);
	
	RooAddModel combinedSignalRes("combinedSignalRes","combinedSignalRes", RooArgList(gaussSigB1,gaussSigB2, gaussSigB3), RooArgList(fres1, fres2, fres3));
	
	RooRealVar fbkg1("fbkg1","fbkg parameter",0.8);
	RooRealVar fbkg2("fbkg2","fbkg parameter",0.2);
	RooRealVar mgBkg("mgBkg","mg2",0);
	RooRealVar sgBkg1("sgBkg1","sg2",1.57);
	RooGaussian gaussBkg1("gaussBkg1","gauss",dt,mgBkg,sgBkg1);

	RooRealVar sgBkg2("sgBkg2","sg2",4.67);
	RooGaussian gaussBkg2("gaussBkg2","gauss",dt,mgBkg,sgBkg2);
	
	RooAddPdf combinedBkg("combinedBkg","combinedBkg", RooArgList(gaussBkg1,gaussBkg2), RooArgList(fbkg1));

	// Construct Bdecay with CP violation
	//RooBCPEffDecay bcp("bcp","bcp", dt, q, tau, dm, w, CPeigen, absLambda, argLambda, effR, dw, truthModel, RooBCPEffDecay::DoubleSided) ;
	RooBCPGenDecay bcp("bcp","bcp", dt, q, tau, dm, w, A, S, dw, effR, combinedSignalRes, RooBCPGenDecay::DoubleSided) ;
	
	RooAddPdf combinedQ("combinedQ","combinedQ", RooArgList(bcp,combinedBkg), RooArgList(fsig1));
	 
	// Generate some data
	if (!data) 
	{
		int nevents = 161./fsignorm;
		nevents = 1356.96/fsignorm; // PHASE III 5 ab^-1 DATASET
		//nevents = 13569.6/fsignorm; // FULL 50ab^-1 DATASET
		nevents = 30000;
		std::cout << " _________________________________________ \n" << std::endl;
		std::cout << "       Generating " << nevents << " Toy MC events" << std::endl;
		std::cout << " _________________________________________ " << std::endl;
		data = combinedQ.generate(RooArgSet(dt,q), nevents);
	}
	data->get(0)->Print();
	//RooFitResult* resb = bcp.fitTo(*data, Save()) ;
	RooFitResult* resb = combinedQ.fitTo(*data, Save()) ;
	// Plot B0 and B0bar tagged data separately
	RooPlot* frame = dt.frame(Title("B decay (B0/B0bar)")) ;

	data->plotOn(frame,Cut("q==q::B0"),MarkerColor(kBlue), MarkerStyle(22)) ;
	//bcp.plotOn(frame, Slice(q,"B0"),LineStyle(kDashed), LineColor(kBlue)) ;
	combinedQ.plotOn(frame, Slice(q,"B0"),LineStyle(kDashed), LineColor(kBlue)) ;

	data->plotOn(frame,Cut("q==q::B0bar"),MarkerColor(kRed), MarkerStyle(23)) ;
	//bcp.plotOn(frame, Slice(q,"B0bar"),LineStyle(kDashed), LineColor(kRed)) ;
	combinedBkg.plotOn(frame, Normalization(0.5-fsignorm/2),LineStyle(kDashed), LineColor(kGray));
	combinedQ.plotOn(frame, Slice(q,"B0bar"),LineStyle(kDashed), LineColor(kRed)) ;
	//combinedSignalRes.plotOn(frame, Normalization(1-fsignorm),LineStyle(kDashed), LineColor(kGray));
	TCanvas* c = new TCanvas("bphysics","bphysics",1000,500) ;
	c->Divide(2,1);
	c->cd(1);
	gPad->SetLeftMargin(0.15) ; frame->GetYaxis()->SetTitleOffset(1.6) ; frame->Draw() ;
	float Arange[2] = {-0.3, 0.3};
	float Srange[2] = {-0.3, 0.3};
	RooPlot* frame2 = new RooPlot(A,S,Arange[0], Arange[1],Srange[0],Srange[1]) ;
	frame2->SetTitle("Ellipse") ;
	//resb->plotOn(frame2,A,S,"ME12ABHV") ;
	resb->plotOn(frame2,A,S,"MEHV") ;
	//new TCanvas("convolution","convolution",500,500);
	c->cd(2);
	gPad->SetLeftMargin(0.15) ; frame2->GetYaxis()->SetTitleOffset(1.4) ; frame2->Draw() ;
	std::cout << "-----------------------------" << std::endl;
	std::cout << "-----------------------------" << std::endl;
	std::cout << "-----------------------------" << std::endl;
	resb->Print();
}
