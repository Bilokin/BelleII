
//
//      	  
//      II 
//                                
//      II  PPPP   HH  HH    CCCC   
//      II  PP  P  HH  HH  CC 
//      II  PPPP   HH  HH  CC
//      II  PP     HHHHHH  CC
//      II  PP     HH  HH    CCCC    STRASBOURG 2017
//      
//        
//                        Author: Bilokin S.    
//

#ifndef	__mbcdeFit__
#define __mbcdeFit__

#include "fitSettings.C"
#include "fitFunctions.C"
#include "plotMCstudy.C"
#include "plotFitResult.C"
#include "sigbkgFitResult.C"
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
	std::cout << "|   FINAL FIT version 0.5    |" << std::endl;
	std::cout << "|____________________________|\n" << std::endl;
	std::cout << "|                            |\n" << std::endl;
	std::cout << "|   BILOKIN S. IPHC FRANCE   |\n" << std::endl;
	std::cout << "|____________________________|\n" << std::endl;
	RooRealVar mbc("mbc","m_{bc} [GeV]",5.2,5.30) ;
	RooRealVar de("de","#Delta E [GeV]",-0.2,0.2) ;
	RooRealVar cs("cs","CSMVA []",0,2.5) ;
	RooRealVar dt("dt","#Delta t [ps]",-20,20) ;
	
	RooRealVar fsig("fsig","#background events",settings.fsig, 0, 1);

	RooCategory q("q","Flavour of the tagged B0") ;
		q.defineType("B0",1) ;
		q.defineType("B0bar",-1) ;
	RooRealVar w("w","flavour mistag rate",0,.5);
	//RooRealVar w("w","flavour mistag rate",0.23);
	RooRealVar A("A","A",0.3,-1.5,1.5);
	RooRealVar S("S","S",0.3,-1.5,1.5);

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
		int nevents = settings.Nevents;// 
		int nexperiments = settings.Nexperiments; //
		//nevents *= 1.25;     // 1 ab-1
		//nevents *= 5; // 5 ab-1
		//nevents *= 5; // 25 ab-1
		//nevents *= 2;    // 50 ab-1
		std::cout << " _________________________________________ \n" << std::endl;
		std::cout << "       Generating " << nevents << " Toy MC events" << std::endl;
		std::cout << " _________________________________________ " << std::endl;
		w.setVal(settings.w);
		w.setConstant();
		std::cout << " _________________________________________ " << std::endl;
		std::cout << " _________________________________________ " << std::endl;
		data = combined->generate(RooArgSet(mbc,de,cs,dt,q), nevents);
		std::cout << " _________________________________________ " << std::endl;
		//data->Print();
		std::cout << " _________________________________________ " << std::endl;
		if (nexperiments) 
		{
			RooMCStudy* mcstudy = new RooMCStudy(*combined,RooArgSet(mbc,de,cs,dt,q),FitModel(*combined)) ;
			mcstudy->generateAndFit( nexperiments, nevents) ;
			plotMCstudy(mcstudy, A,S);
			gDirectory->Add(mcstudy) ;
		}
		//return;
	}
	else 
	{
		std::cout << "Importing tree: ";// << std::endl;
		data = new RooDataSet(name.c_str(), name.c_str(), RooArgSet(mbc,de,cs,dt,q,w), Import(*tree));
		data->Print();
		std::cout << "Done!" << std::endl;
	}
	RooFitResult* resb = combined->fitTo(*data, Save(), Silence()) ;

	plotFitResult(combined, bkgpdf, sigpdf, data, mbc, de, dt, cs, q);
	sigbkgFitResult(combined, bkgpdf, sigpdf, data, mbc, de, dt, cs, q);
	plotAsymmetryResult(combined, bkgpdf, sigpdf, data, resb, A, S, dt, q);
		
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
