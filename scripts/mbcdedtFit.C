
#ifndef	__mbcdeFit__
#define __mbcdeFit__

#include "fitSettings.C"
#include "fitFunctions.C"
#ifndef __MYROOFIT__
#define __MYROOFIT__

#include "RooGlobalFunc.h"
#include "RooRealVar.h"
#include "RooDataSet.h"
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


void mbcdedtFit(TTree* tree, fitSettings settings, bool showSecCanvas = true)
{
	std::cout << " ____________________________" << std::endl;
	std::cout << "|                            |" << std::endl;
	std::cout << "|   FINAL FIT version 0.4    |" << std::endl;
	std::cout << "|____________________________|\n" << std::endl;
	RooRealVar mbc("mbc","m_{bc} [GeV]",5.20,5.30) ;
	RooRealVar de("de","#Delta E [GeV]",-0.2,0.2) ;
	RooRealVar cs("cs","CSMVA []",0,2) ;
	RooRealVar dt("dt","#Delta t [ps]",-20,20) ;
	
	RooRealVar fsig("fsig","#background events",0.05, 0, 1);

	RooCategory q("q","Flavour of the tagged B0") ;
		q.defineType("B0",1) ;
		q.defineType("B0bar",-1) ;
	RooRealVar w("w","flavour mistag rate",0,.5);
	//RooRealVar w("w","flavour mistag rate",0.25);
	RooRealVar A("A","A",0.3,-1,1);
	RooRealVar S("S","S",0.3,-1,1);

	//fitSettings settings = getStdSettings();
	bool fixBkgShape = false;
	
	RooAbsPdf * mbcbkg = getMbcBkg(settings, mbc, fixBkgShape);
	RooAbsPdf * debkg = getDeBkg(settings, de, fixBkgShape);
	RooAbsPdf * csbkg = getCsBkg(settings, cs, fixBkgShape);
	RooAbsPdf * dtbkg = getDeltaTBkg(settings, dt, fixBkgShape);
	
	RooAbsPdf * mbcsignal = getMbcSignal(settings, mbc);
	RooAbsPdf * designal = getDeSignal(settings, de);
	RooAbsPdf * cssignal = getCsSignal(settings, cs);
	RooAbsPdf * dtsignal = getDeltaTSignal(settings, dt,q,w,A,S);
	
	//RooAbsPdf * function = getMbcDe(settings, mbc, de, fbkg);
	// --- Build Product PDF ---
	RooProdPdf * bkgpdf = new RooProdPdf("bkg","bkg",RooArgList(*mbcbkg, *debkg, *csbkg, *dtbkg));
	RooProdPdf * sigpdf = new RooProdPdf("signal","signal",RooArgSet(*mbcsignal, *designal, *cssignal, *dtsignal));
	//RooProdPdf * bkgpdf = new RooProdPdf("bkg","bkg",RooArgList(*mbcbkg, *debkg,  *dtbkg));
	//RooProdPdf * sigpdf = new RooProdPdf("signal","signal",RooArgSet(*mbcsignal, *designal,  *dtsignal));
	
	RooAddPdf * combined = new RooAddPdf("combined","combined", RooArgList(*sigpdf, *bkgpdf), fsig);
	RooDataSet* data = NULL;
	RooDataSet* dtdata = NULL;
	bool useToyMC = (tree)? 0:1;
	string name = "MC";
	
	if (useToyMC) 
	{
		int nevents = 2846; //0.555 ab-1
		nevents *= 9.001; //5 ab-1
		nevents *= 10;    //50 ab-1
		std::cout << " _________________________________________ \n" << std::endl;
		std::cout << "       Generating " << nevents << " Toy MC events" << std::endl;
		std::cout << " _________________________________________ " << std::endl;
		w.setVal(0.232);
		w.setConstant();
		std::cout << " _________________________________________ " << std::endl;
		std::cout << " _________________________________________ " << std::endl;
		data = combined->generate(RooArgSet(mbc,de,cs,dt,q), nevents);
		std::cout << " _________________________________________ " << std::endl;
		data->Print();
		std::cout << " _________________________________________ " << std::endl;
	}
	else 
	{
		std::cout << "Importing tree: ";// << std::endl;
		data = new RooDataSet(name.c_str(), name.c_str(), RooArgSet(mbc,de,cs,dt,q,w), Import(*tree));
		data->Print();
		std::cout << "Done!" << std::endl;
	}
	RooFitResult* resb = combined->fitTo(*data, Save()) ;

	mbc.setBins(50) ;
	cs.setBins(50) ;
	de.setBins(25) ;
	dt.setBins(50) ;
	
	RooPlot* frame = mbc.frame(Title("M_{bc}")) ;
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
	cs.setRange("Signal",0.5,2);
	dt.setRange(-10,10);
	TLatex* txt = new TLatex(0.2,0.85,"Signal region") ;
	txt->SetNDC();
	txt->SetTextSize(0.05) ;
	txt->SetTextColor(kGray) ;
	dtframe->addObject(txt) ;
	data->plotOn(dtframe, MarkerStyle(22), CutRange("Signal")) ;
        combined->plotOn(dtframe,  ProjectionRange("Signal")) ;
        combined->plotOn(dtframe, ProjectionRange("Signal"), Components(*bkgpdf), LineStyle(kDashed), LineColor(kGray)) ;
        combined->plotOn(dtframe, ProjectionRange("Signal"), Components(*sigpdf), Slice(q,"B0"),LineStyle(kDashed), LineColor(kBlue)) ;
        combined->plotOn(dtframe, ProjectionRange("Signal"), Components(*sigpdf), Slice(q,"B0bar"),LineStyle(kDashed), LineColor(kRed)) ;

	float Arange[2] = {-0.3, 0.3};
	float Srange[2] = {-0.3, 0.3};
	RooPlot* frameres = new RooPlot(A,S,Arange[0], Arange[1],Srange[0],Srange[1]) ;
	frameres->SetTitle("");
	resb->plotOn(frameres,A,S,"MEHV") ;

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
	//gPad->SetLeftMargin(0.15) ; frameres->GetYaxis()->SetTitleOffset(1.6) ; frameres->Draw() ;
		
	TCanvas* c2 = new TCanvas("bphysics2","bphysics",400,400) ;
	gPad->SetLeftMargin(0.15) ; frameres->GetYaxis()->SetTitleOffset(1.6) ; frameres->Draw() ;
	std::cout << "-----------------------------" << std::endl;
	std::cout << "-----------------------------" << std::endl;
	std::cout << "-----------------------------" << std::endl;
	resb->Print();
}

void mbcdedtFit()
{
	mbcdedtFit(NULL, getStdSettings(), false);
}
#endif
