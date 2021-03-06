
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
//void mbcdetest(string filename = "root/mysignal-csmva-merged.root", string filenameBkg = "root/tmp-fullrun-csmva1.root")
void mbcdetest_contevent(string filename = "tmp-branch.root")
{
	fitSettings settings = getStdSettings();
	settings.useConstMistag = true;
	RooRealVar mbc("mbc","m_{bc} [GeV]",5.20,5.30) ;
	RooRealVar de("de","#Delta E [GeV]",-0.2,0.2) ;
	RooRealVar cs("cs","CSMVA []",0,2) ;
	RooRealVar dt("dt","dt [ps]",-10,10) ;
	RooCategory q("q","Flavour of the tagged B0") ;
		q.defineType("B0",1) ;
		q.defineType("B0bar",-1) ;
	RooRealVar A("A","A",0);
	RooRealVar S("S","S",0);
	RooRealVar w("w","flavour mistag rate",0.2);
	RooRealVar wbkg("w","flavour mistag rate",0.1);
	
	TFile * file = TFile::Open(filename.c_str());
	TTree* B0Signal = (TTree*)file->Get("B0Signal");
	string bkgcut= "(!B0_isContinuumEvent-B0_isSignal)";

	TH1F * mbcBkgHist = new TH1F("mbcBkgHist", ";M [GeV]", 50,5.2,5.3);
	TH1F * deBkgHist = new TH1F("deBkgHist", ";de [GeV]", 25,-0.2,0.2);
	TH1F * csBkgHist = new TH1F("csBkgHist", ";cs [GeV]", 50,0,2);
	TH1F * dtBkgHist = new TH1F("dtBkgHist", ";dt [ps]", 50,-10,10);
	B0Signal->Project("mbcBkgHist","B0_mbc",(bkgcut).c_str());
	B0Signal->Project("deBkgHist","B0_deltae",(bkgcut).c_str());
	B0Signal->Project("csBkgHist","B0_CSMVA",(bkgcut).c_str());
	B0Signal->Project("dtBkgHist","B0_DeltaT",(bkgcut).c_str());

	RooDataHist mbcBkgRooHist("mbcBkgRooHist","mbcBkgRooHist",mbc,Import(*mbcBkgHist)) ;
	RooDataHist deBkgRooHist("deBkgRooHist","deBkgRooHist",de,Import(*deBkgHist)) ;
	RooDataHist csBkgRooHist("csBkgRooHist","csBkgRooHist",cs,Import(*csBkgHist)) ;
	RooDataHist dtBkgRooHist("dtBkgRooHist","dtBkgRooHist",dt,Import(*dtBkgHist)) ;
	
	RooRealVar fsig1("fsig1","fsig parameter", 0.5);

// --- Build Gaussian signal PDF ---
	RooRealVar sigmean("sigmean","B^{#pm} mass",5.2795, 5.2, 5.3) ;
	RooRealVar sigsigma("sigsigma","B^{#pm} width",0.0027,0.000001,1.) ;
	RooRealVar sigalpha("sigalpha","B^{#pm} width",0,4.) ;
	RooRealVar sign("sign","B^{#pm} width",0,20.) ;
	RooCBShape CBall("CBall", "Crystal Ball shape", mbc, sigmean, sigsigma, sigalpha, sign);
	
// --- Build Gaussian signal PDF ---
	RooGaussian gauss("gauss","gaussian PDF",mbc,sigmean,sigsigma) ;
	
// --- Build Argus background PDF ---
	RooRealVar argpar("argpar","argus shape parameter",-80,-100.,-20.) ;
	RooArgusBG argus("argus","Argus PDF",mbc,RooConst(5.29),argpar) ;
	
// --- Build Chebychev background PDF ---
	RooRealVar c0("c0","coefficient #0", 1.0,-1.,1.) ;
	RooRealVar c1("c1","coefficient #1", 0.1,-1.,1.) ;
	RooRealVar c2("c2","coefficient #2",-0.1,-1.,1.) ;
	RooChebychev chebychev("chebychev","background p.d.f.",de,RooArgList(c0,c1)) ; 
	
// --- Construct signal+background PDF ---
	RooRealVar nsig("nsig","#signal events",200,0.,10000) ;
	RooRealVar nbkg("nbkg","#background events",800,0.,10000) ;
	RooAddPdf sum("sum","g+a",RooArgList(gauss,argus),RooArgList(nsig,nbkg)) ;
	
// --- Build Gaussian signal PDF ---
	RooRealVar csmean1("csmean1","B^{#pm} mass",0.2795, -3, 3) ;
	RooRealVar cssigma1("cssigma1","B^{#pm} width",0.27,0.001,10.) ;
	RooRealVar csmean2("csmean2","B^{#pm} mass",0.2795, -3, 3) ;
	RooRealVar cssigma2("cssigma2","B^{#pm} width",0.27,0.001,10.) ;
	//RooRealVar csalpha("csalpha","B^{#pm} width",1,20.) ;
	//RooRealVar csn("csn","B^{#pm} width",0,20.) ;
	//RooCBShape csCBall("csCBall", "Crystal Ball shape", cs, csmean, cssigma, csalpha, csn);
	RooRealVar fcssig("fcssig","#signal events",0.5,0.0001,1) ;
        RooGaussian gaussCs1("gaussCs1","gaussian PDF",cs,csmean1,cssigma1) ;
        RooGaussian gaussCs2("gaussCs2","gaussian PDF",cs,csmean2,cssigma2) ;
	RooAddPdf csCBall("csCBall","g+a",RooArgList(gaussCs1,gaussCs2),RooArgList(fcssig)) ;
	
// --- Build Gaussian signal PDF ---
	RooRealVar csbkgmean1("csbkgmean1","B^{#pm} mass",0.7,0.4,1.) ;
	RooRealVar csbkgsigma1("csbkgsigma1","B^{#pm} width",0.1,0.001,10.) ;
        RooGaussian gaussCsBkg1("gaussCsBkg1","gaussian PDF",cs,csbkgmean1,csbkgsigma1) ;
  RooRealVar csbkgmean2("csbkgmean2","B^{#pm} mass",0.3,0.,1.) ;
	RooRealVar csbkgsigma2("csbkgsigma2","B^{#pm} width",0.6,0.001,10.) ;
        RooGaussian gaussCsBkg2("gaussCsBkg2","gaussian PDF",cs,csbkgmean2,csbkgsigma2) ;
  RooRealVar fcssig2("fcssig2","#signal events",0.5,0.0001,1) ;
	RooAddPdf csGauss("csGauss2","g+g",RooArgList(gaussCsBkg1,gaussCsBkg2),RooArgList(fcssig2));
	
	RooRealVar csbkgpar("csbkgpar","B^{#pm} mass",0.2795, -300, 300) ;
	RooExponential csbkg("csbkg", "csbkg", cs, csbkgpar);
	
// --- Generate a toyMC sample from composite PDF ---
	
	RooAbsPdf * dtsignal = getDeltaTSignal(settings, dt,q,w,A,S);
	RooAbsPdf * dtbkg = getDeltaTBkg(settings, dt, false);
	//RooAbsPdf * dtbkg = getDeltaTSignal(settings, dt,q,wbkg,A,S,false);
// --- Generate a toyMC sample from composite PDF ---
	RooDataSet *data = sum.generate(mbc,2000) ;
	
	
	
	
// --- Perform extended ML fit of composite PDF to toy data ---
	//sum.fitTo(*data,Extended()) ;
	RooFitResult* resMbcBkg = sum.fitTo(mbcBkgRooHist,Save());
	RooFitResult* resDeBkg = chebychev.fitTo(deBkgRooHist,Save());
	RooFitResult* resCsBkg = csGauss.fitTo(csBkgRooHist,Save());
	RooFitResult* resDtBkg = dtbkg->fitTo(dtBkgRooHist,Save());
	
	
	
	
// --- Plot toy data and composite PDF overlaid ---
	TCanvas* c = new TCanvas("bphysics bkg continuum","bphysics bkg continuum",1000,1000) ;
	c->Divide(2,2);
	c->cd(1);
	RooPlot* mbcframe2 = mbc.frame() ;
	mbcBkgRooHist.plotOn(mbcframe2) ;
	sum.plotOn(mbcframe2) ;
	gPad->SetLeftMargin(0.15) ; mbcframe2->GetYaxis()->SetTitleOffset(1.6) ; mbcframe2->Draw() ;
	//mbcHist->Draw("hesame");
	c->cd(2);
	RooPlot* deframe2 = de.frame() ;
	deBkgRooHist.plotOn(deframe2) ;
	chebychev.plotOn(deframe2) ;
	gPad->SetLeftMargin(0.15) ; deframe2->GetYaxis()->SetTitleOffset(1.6) ; deframe2->Draw() ;
	c->cd(4);
	RooPlot* csframe2 = cs.frame() ;
	csBkgRooHist.plotOn(csframe2) ;
	csGauss.plotOn(csframe2) ;
	gPad->SetLeftMargin(0.15) ; csframe2->GetYaxis()->SetTitleOffset(1.6) ; csframe2->Draw() ;
	c->cd(3);
	RooPlot* dtframe2 = dt.frame() ;
	dtBkgRooHist.plotOn(dtframe2) ;
	dtbkg->plotOn(dtframe2) ;
	gPad->SetLeftMargin(0.15) ; dtframe2->GetYaxis()->SetTitleOffset(1.6) ; dtframe2->Draw() ;
	std::cout << "-----------------------------------" << std::endl;
	std::cout << "-----------------------------------" << std::endl;
	std::cout << "-----------------------------------" << std::endl;
	resMbcBkg->Print();
	std::cout << "-----------------------------------" << std::endl;
	resDeBkg->Print();
	std::cout << "-----------------------------------" << std::endl;
	std::cout << "-----------------------------------" << std::endl;
	resCsBkg->Print();
	std::cout << "-----------------------------------" << std::endl;
	resDtBkg->Print();
	std::cout << "-----------------------------------" << std::endl;
	//std::cout << "Nevents: " << csHist->GetEntries() << std::endl;
	std::cout << "Width: " << dtBkgHist->GetRMS() << std::endl;
}
/*
fitSettings getStdSettings()
{
	fitSettings settings;
	settings.mbcSigPar = {5.27952, 3.12605e-03, 1.2978, 1.837e1};
	settings.mbcBkgPar = {5.29};
	settings.deSigPar = { -0.01, 4.16911e-02, 6.16997e-01, 1.0};
	return settings;
}*/
