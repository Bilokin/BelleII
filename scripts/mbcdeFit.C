

#include "fitSettings.C"
#include "fitFunctions.C"
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

fitSettings getStdSettings()
{
	fitSettings settings;
	settings.mbcSigPar = {5.27952, 3.12605e-03, 1.2978, 1.837e1};
	settings.mbcBkgPar = {5.29};
	settings.deSigPar = { -0.01, 4.16911e-02, 6.16997e-01, 1.0};
	settings.deBkgPar = {-3.99680e-01, 8.18152e-02, -2.90946e-02};
	return settings;
}

void mbcdeFit(string filename = "signal-veto2/mergegrids.root", string filenameBkg = "ccbar-veto2/mergegrids7-new.root")
{

	RooRealVar mbc("mbc","m_{bc} [GeV]",5.20,5.30) ;
	RooRealVar de("de","#Delta E [GeV]",-0.2,0.2) ;
	fitSettings settings = getStdSettings();
	RooAbsPdf * function = getMbcDe(settings, mbc, de);
	RooDataSet* data = NULL;
	bool useToyMC = true;
	
	if (useToyMC) 
	{
		int nevents = 2750;
		data = function->generate(RooArgSet(mbc,de),nevents);
	}
	
	RooFitResult* resb = function->fitTo(*data, Save()) ;

	mbc.setBins(40) ;
	de.setBins(20) ;
	RooPlot* frame = mbc.frame(Title("M_{bc}")) ;
	data->plotOn(frame, MarkerStyle(22)) ;
        function->plotOn(frame) ;

	TCanvas* c = new TCanvas("bphysics","bphysics",1000,500) ;
	c->Divide(2,1);
	c->cd(1);
	gPad->SetLeftMargin(0.15) ; frame->GetYaxis()->SetTitleOffset(1.6) ; frame->Draw() ;
	RooPlot* deframe = de.frame(Title("#Delta E")) ;
	data->plotOn(deframe, MarkerStyle(22)) ;
        function->plotOn(deframe) ;
	c->cd(2);
	gPad->SetLeftMargin(0.15) ; deframe->GetYaxis()->SetTitleOffset(1.6) ; deframe->Draw() ;
	std::cout << "-----------------------------" << std::endl;
	std::cout << "-----------------------------" << std::endl;
	std::cout << "-----------------------------" << std::endl;
	resb->Print();
}
