
//
//      	  
//      II 
//                                
//      II  PPPP   HH  HH    CCCC   
//      II  PP  P  HH  HH  CC 
//      II  PPPP   HHHHHH  CC
//      II  PP     HH  HH  CC
//      II  PP     HH  HH    CCCC    STRASBOURG 2017
//      
//        
//                        Author: Bilokin S.    
//

//To launch one Toy MC experiment using standard parameters:
//.x mbcdedtFit.C()

//To change the parameters one needs to modify getStdSettings() method at
//the end of fitSettings.C

#ifndef	__mbcdeFit__
#define __mbcdeFit__

#include "fitSettings.C"
#include "fitFunctions.C"
#include "plotMCstudy.C"
#include "plotFitResult.C"
#include "sigbkgFitResult.C"
//#include "1plotFitResult.C"
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
	std::cout << "|____________________________|" << std::endl;
	std::cout << "|                            |" << std::endl;
	std::cout << "|   BILOKIN S. IPHC FRANCE   |" << std::endl;
	std::cout << "|____________________________|" << std::endl;
	
	
	RooRealVar mbc("mbc","m_{bc} [GeV]",5.2,5.30) ;//²beam constrained
	RooRealVar de("de","#Delta E [GeV]",-0.2,0.2) ;//²b meson & center of mass
	RooRealVar cs("cs","CSMVA []",0,2.5) ;//²continuum process (specific to bfactor, only want bbar decay but we have many of them; cross section(uu)=1.6pb (upsilon4s)=1.1pb,  (ddbar)=0.4pb; uu/cc on veut pas mais same rate; 2dd -> homogene ds l'espace alors que uu c'est jet; eventshapevariable pr suppr les uu-> 30 variables du coup CSMVA pr les virer, use 12var, trained for MVA-addition TMVA
	RooRealVar dt("dt","#Delta t [ps]",-20,20) ;
	RooRealVar fsig("fsig","#background events",settings.fsig, 0.0, 1.0);
	RooRealVar fsigBB("fsigBB","#BBbackground events",settings.fsigBB);

	RooCategory q("q","Flavour of the tagged B0") ;
		q.defineType("B0",1) ;
		q.defineType("B0bar",-1) ;
	RooRealVar w("w","flavour mistag rate",0,.5);
	//RooRealVar w("w","flavour mistag rate",0.23);
	RooRealVar A("A","A",0.3,-1.5,1.5);
	RooRealVar S("S","S",0.3,-1.5,1.5);

	//fitSettings settings = getStdSettings();
	bool fixBkgShape = false;
	
	RooAbsPdf * mbcbkg = getMbcBkg(settings, mbc, fixBkgShape); //argpar
	RooAbsPdf * debkg = getDeBkg(settings, de, fixBkgShape);		//c0,c1
	RooAbsPdf * csbkg = getCsBkg(settings, cs, fixBkgShape);		//csbkgsigma1
	RooAbsPdf * dtbkg = getDeltaTBkg(settings, dt, fixBkgShape);//fbkg1,2,sgBkgPar
	
	RooAbsPdf * mbcBBbkg = getMbcBBbkg(settings, mbc);
	RooAbsPdf * deBBbkg = getDeBBbkg(settings, de);
	RooAbsPdf * csBBbkg = getCsBBbkg(settings, cs);
	RooAbsPdf * dtBBbkg = getDeltaTBBbkg(settings, dt);
	
	RooAbsPdf * mbcsignal = getMbcSignal(settings, mbc);
	RooAbsPdf * designal = getDeSignal(settings, de);
	RooAbsPdf * cssignal = getCsSignal(settings, cs);
	RooAbsPdf * dtsignal = getDeltaTSignal(settings, dt,q,w,A,S);
	
	// --- Build Product PDF ---
	RooProdPdf * bkgpdf = new RooProdPdf("bkg","bkg",RooArgList(*mbcbkg, *debkg, *csbkg, *dtbkg));
	RooProdPdf * sigpdf = new RooProdPdf("signal","signal",RooArgSet(*mbcsignal, *designal, *cssignal, *dtsignal));//c'est des densité de proba du coup ok if no correl
	RooProdPdf * BBbkgpdf = new RooProdPdf("BBbkg","BBbkg",RooArgList(*mbcBBbkg, *deBBbkg, *csBBbkg, *dtBBbkg));
	
	RooAddPdf * combined = new RooAddPdf("combinedBB","combinedBB", RooArgList(*sigpdf, *bkgpdf, *BBbkgpdf), RooArgList(fsig,fsigBB));
	
	RooDataSet* data = NULL;
	RooDataSet* dtdata = NULL;
	bool useToyMC = (tree)? 0:1;
	string name = "MC";
	
	if (useToyMC) 
	{
		int nevents = settings.Nevents;// 
		int nexperiments = settings.Nexperiments; //
		//nevents *= 1.25; 	// 1 ab-1
		//nevents *= 5; 		// 5 ab-1
		//nevents *= 5; 		// 25 ab-1
		//nevents *= 2;    	// 50 ab-1
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
			RooMCStudy* mcstudy = new RooMCStudy(*combined,RooArgSet(mbc,de,cs,dt,q),FitModel(*combined),Silence()) ;
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

	plotFitResult(combined, bkgpdf, BBbkgpdf, sigpdf, data, mbc, de, dt, cs, q);
	//sigbkgFitResult(combined, bkgpdf, BBbkgpdf, sigpdf, data, mbc, de, dt, cs, q);
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
