

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

fitSettingsMbcdE getStdSettings()
{
	fitSettingsMbcdE settings;
	settings.mbcSigPar = {5.27952, 3.12605e-03, 1.2978, 1.837e1};
	settings.mbcBkgPar = {5.29};
	settings.deSigPar = { -0.01, 4.16911e-02, 6.16997e-01, 1.0};
	settings.deBkgPar = {-3.99680e-01, 8.18152e-02, -2.90946e-02};
	return settings;
}
RooAbsPdf * getMbc(fitSettingsMbcdE & set, RooRealVar & mbc, RooRealVar* fbkg)
{
// --- Build Mbc CB signal PDF ---
	RooRealVar*  sigmean = new RooRealVar("sigmean","B^{#pm} mass",set.mbcSigPar[0]);//, 5.2, 5.3) ;
	RooRealVar * sigsigma = new RooRealVar("sigsigma","B^{#pm} width",set.mbcSigPar[1]);//,0.000001,1.) ;
	RooRealVar * sigalpha = new RooRealVar("sigalpha","B^{#pm} width",set.mbcSigPar[2]);//,1,4.) ;
	RooRealVar * sign = new RooRealVar("sign","B^{#pm} width", set.mbcSigPar[3]);//,1,20.) ;
	RooCBShape * CBall = new RooCBShape("CBall", "Crystal Ball shape", mbc, *sigmean, *sigsigma, *sigalpha, *sign);
// --- Build Argus background PDF ---
	RooRealVar * argpar = new RooRealVar("argpar","argus shape parameter",-30.0,-100.,-1.) ;
	RooArgusBG * argus = new RooArgusBG("argus","Argus PDF",mbc,RooConst(set.mbcBkgPar[0]),*argpar) ;
// --- Build signal+ background PDF ---
	RooArgList * mbclist = new RooArgList(*CBall,*argus);
	RooAddPdf * mbcsum = new RooAddPdf("mbcsum","g+a mbc", *mbclist,RooArgList(*fbkg)) ;
	return mbcsum;
}
RooAbsPdf * getDe(fitSettingsMbcdE & set,  RooRealVar & de, RooRealVar* fbkg)
{
// --- Build deltaE CB signal PDF ---
	RooRealVar * demean = new RooRealVar("demean","B^{#pm} mass", set.deSigPar[0]) ;
	RooRealVar * desigma = new RooRealVar("desigma","B^{#pm} width",set.deSigPar[1]);//,0.001,1.) ;
	RooRealVar * dealpha = new RooRealVar("dealpha","B^{#pm} width", set.deSigPar[2]);//,0,2.) ;
	RooRealVar * den = new RooRealVar("den","B^{#pm} width", set.deSigPar[3]);//,0.5,20.) ;
	RooCBShape * deCBall = new RooCBShape("deCBall", "Crystal Ball shape", de, *demean, *desigma, *dealpha, *den);
// --- Build Chebychev background PDF ---
	RooRealVar * c0 = new RooRealVar("c0","coefficient #0", set.deBkgPar[0],-1.,1.) ;
	RooRealVar * c1 = new RooRealVar("c1","coefficient #1", set.deBkgPar[1],-1.,1.) ;
	RooRealVar * c2 = new RooRealVar("c2","coefficient #2", set.deBkgPar[2],-1.,1.) ;
	RooChebychev * chebychev = new RooChebychev("chebychev","background p.d.f.",de,RooArgList(*c0,*c1,*c2)) ; 

// --- Build signal+ background PDF ---
	RooArgList * delist = new RooArgList(*deCBall,*chebychev);
	RooAddPdf * desum = new RooAddPdf("desum","g+a de", *delist,RooArgList(*fbkg)) ;
	return desum;
}
RooAbsPdf * getMbcDe(fitSettingsMbcdE & set, RooRealVar & mbc, RooRealVar & de)
{
	//RooRealVar* nsig = new RooRealVar("nsig","#signal events",100,0.,10000) ;
	//RooRealVar* nbkg = new RooRealVar("nbkg","#background events",800,0.,10000);
	RooRealVar* fbkg = new RooRealVar("fbkg","#background events",0.076,0.,1);
	RooAbsPdf * mbcsum = getMbc(set, mbc, fbkg);
	RooAbsPdf * desum = getDe(set, de, fbkg);
// --- Build Product PDF ---
	RooProdPdf * result = new RooProdPdf("result","de*mbc",*mbcsum, *desum);
	return result;
}

void mbcdeFit(string filename = "signal-veto2/mergegrids.root", string filenameBkg = "ccbar-veto2/mergegrids7-new.root")
{

	RooRealVar mbc("mbc","m_{bc} [GeV]",5.20,5.30) ;
	RooRealVar de("de","#Delta E [GeV]",-0.2,0.2) ;
	fitSettingsMbcdE settings = getStdSettings();
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
