
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
void mbcdeFit_test(string filename = "signal-xsd.root", string filenameBkg = "signal-xsd.root")
{
	RooRealVar mbc("mbc","m_{bc} [GeV]",5.20,5.30) ;
	RooRealVar de("de","#Delta E [GeV]",-0.2,0.2) ;
	RooRealVar cs("cs","CSMVA []",0,2) ;

	TFile * file = TFile::Open(filename.c_str());
	TTree* B0Signal = (TTree*)file->Get("B0Signal");
	string mccut= "(B0_isSignal) &&";
	string bkgcut= "(!B0_isSignal) &&";
	string cut = getBasicCuts(1,"Xsd");
	TH1F * mbcHist = new TH1F("mbcHist", ";M [GeV]", 100,5.2,5.3);
	TH1F * deHist = new TH1F("deHist", ";M [GeV]", 50,-0.2,0.2);
	TH1F * csHist = new TH1F("csHist", ";cs [GeV]", 100,0,2);
	B0Signal->Project("mbcHist","B0_mbc",(mccut+cut).c_str());
	B0Signal->Project("deHist","B0_deltae",(mccut+cut).c_str());
	B0Signal->Project("csHist","B0_CSMVA",(mccut+cut).c_str());

	RooDataHist deRooHist("deRooHist","deRooHist",de,Import(*deHist)) ;
	RooDataHist mbcRooHist("mbcRooHist","mbcRooHist",mbc,Import(*mbcHist)) ;
	RooDataHist csRooHist("csRooHist","csRooHist",cs,Import(*csHist)) ;

	TFile * filebkg = TFile::Open(filenameBkg.c_str());
	TTree* B0SignalBkg = (TTree*)filebkg->Get("B0Signal");
	TH1F * mbcBkgHist = new TH1F("mbcBkgHist", ";M [GeV]", 100,5.2,5.3);
	TH1F * deBkgHist = new TH1F("deBkgHist", ";de [GeV]", 100,-0.2,0.2);
	TH1F * csBkgHist = new TH1F("csBkgHist", ";cs [GeV]", 100,0,2);
	B0SignalBkg->Project("mbcBkgHist","B0_mbc",(bkgcut+cut).c_str());
	B0SignalBkg->Project("deBkgHist","B0_deltae",(bkgcut+cut).c_str());
	B0SignalBkg->Project("csBkgHist","B0_CSMVA",(bkgcut+cut).c_str());

	RooDataHist mbcBkgRooHist("mbcBkgRooHist","mbcBkgRooHist",mbc,Import(*mbcBkgHist)) ;
	RooDataHist deBkgRooHist("deBkgRooHist","deBkgRooHist",de,Import(*deBkgHist)) ;
	RooDataHist csBkgRooHist("csBkgRooHist","csBkgRooHist",cs,Import(*csBkgHist)) ;
	
	RooRealVar fsig1("fsig1","fsig parameter", 0.5);
// --- Build Gaussian signal PDF ---
	RooRealVar demean("demean","B^{#pm} mass",-0.01) ;
	RooRealVar desigma("desigma","B^{#pm} width",0.04,0.001,1.) ;
	RooRealVar dealpha("dealpha","B^{#pm} width",0,2.) ;
	RooRealVar den("den","B^{#pm} width",0.5,20.) ;
	RooCBShape deCBall("deCBall", "Crystal Ball shape", de, demean, desigma, dealpha, den);
// --- Build Gaussian signal PDF ---
	RooRealVar sigmean("sigmean","B^{#pm} mass",5.2795, 5.2, 5.3) ;
	RooRealVar sigsigma("sigsigma","B^{#pm} width",0.0027,0.000001,1.) ;
	RooRealVar sigalpha("sigalpha","B^{#pm} width",1,4.) ;
	RooRealVar sign("sign","B^{#pm} width",1,20.) ;
	RooCBShape CBall("CBall", "Crystal Ball shape", mbc, sigmean, sigsigma, sigalpha, sign);
// --- Build Gaussian signal PDF ---
	RooGaussian gauss("gauss","gaussian PDF",mbc,sigmean,sigsigma) ;
// --- Build Argus background PDF ---
	RooRealVar argpar("argpar","argus shape parameter",-20.0,-100.,-1.) ;
	RooArgusBG argus("argus","Argus PDF",mbc,RooConst(5.29),argpar) ;
// --- Build Chebychev background PDF ---
	RooRealVar c0("c0","coefficient #0", 1.0,-1.,1.) ;
	RooRealVar c1("c1","coefficient #1", 0.1,-1.,1.) ;
	RooRealVar c2("c2","coefficient #2",-0.1,-1.,1.) ;
	RooChebychev chebychev("chebychev","background p.d.f.",de,RooArgList(c0,c1,c2)) ; 
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
	RooRealVar csbkgmean1("csbkgmean1","B^{#pm} mass",0.2795, -3, 3) ;
	RooRealVar csbkgsigma1("csbkgsigma1","B^{#pm} width",0.27,0.001,10.) ;
        RooGaussian gaussCsBkg1("gaussCsBkg1","gaussian PDF",cs,csbkgmean1,csbkgsigma1) ;
	
	RooRealVar csbkgpar("csbkgpar","B^{#pm} mass",0.2795, -300, 300) ;
	RooExponential csbkg("csbkg", "csbkg", cs, csbkgpar);
// --- Generate a toyMC sample from composite PDF ---
	RooDataSet *data = sum.generate(mbc,2000) ;
// --- Perform extended ML fit of composite PDF to toy data ---
	//sum.fitTo(*data,Extended()) ;
	RooFitResult* resMbcSig = CBall.fitTo(mbcRooHist,Save());
	RooFitResult* resDeSig = deCBall.fitTo(deRooHist,Save());
	RooFitResult* resMbcBkg = argus.fitTo(mbcBkgRooHist,Save());
	RooFitResult* resDeBkg = chebychev.fitTo(deBkgRooHist,Save());
	RooFitResult* resCsSig = csCBall.fitTo(csRooHist,Save());
	RooFitResult* resCsBkg = gaussCsBkg1.fitTo(csBkgRooHist,Save());
// --- Plot toy data and composite PDF overlaid ---
	TCanvas* c = new TCanvas("bphysics","bphysics",1000,1000) ;
	c->Divide(2,3);
	c->cd(1);
	RooPlot* mbcframe = mbc.frame() ;
	//data->plotOn(mbcframe) ;
	//sum.plotOn(mbcframe) ;
	//sum.plotOn(mbcframe,Components(argus),LineStyle(kDashed)) ; 
	mbcRooHist.plotOn(mbcframe) ;
	CBall.plotOn(mbcframe) ;
	gPad->SetLeftMargin(0.15) ; mbcframe->GetYaxis()->SetTitleOffset(1.6) ; mbcframe->Draw() ;
	
	c->cd(2);
	RooPlot* mbcframe2 = mbc.frame() ;
	mbcBkgRooHist.plotOn(mbcframe2) ;
	argus.plotOn(mbcframe2) ;
	gPad->SetLeftMargin(0.15) ; mbcframe2->GetYaxis()->SetTitleOffset(1.6) ; mbcframe2->Draw() ;
	//mbcHist->Draw("hesame");
	c->cd(3);
	RooPlot* deframe = de.frame() ;
	deRooHist.plotOn(deframe) ;
	deCBall.plotOn(deframe) ;
	gPad->SetLeftMargin(0.15) ; deframe->GetYaxis()->SetTitleOffset(1.6) ; deframe->Draw() ;
	c->cd(4);
	RooPlot* deframe2 = de.frame() ;
	deBkgRooHist.plotOn(deframe2) ;
	chebychev.plotOn(deframe2) ;
	gPad->SetLeftMargin(0.15) ; deframe2->GetYaxis()->SetTitleOffset(1.6) ; deframe2->Draw() ;
	c->cd(5);
	RooPlot* csframe = cs.frame() ;
	csRooHist.plotOn(csframe) ;
	csCBall.plotOn(csframe) ;
	gPad->SetLeftMargin(0.15) ; csframe->GetYaxis()->SetTitleOffset(1.6) ; csframe->Draw() ;
	c->cd(6);
	RooPlot* csframe2 = cs.frame() ;
	csBkgRooHist.plotOn(csframe2) ;
	gaussCsBkg1.plotOn(csframe2) ;
	gPad->SetLeftMargin(0.15) ; csframe2->GetYaxis()->SetTitleOffset(1.6) ; csframe2->Draw() ;
	std::cout << "-----------------------------------" << std::endl;
	resMbcSig->Print();
	std::cout << "-----------------------------------" << std::endl;
	resDeSig->Print();
	std::cout << "-----------------------------------" << std::endl;
	resMbcBkg->Print();
	std::cout << "-----------------------------------" << std::endl;
	resDeBkg->Print();
	std::cout << "-----------------------------------" << std::endl;
	resCsSig->Print();
	std::cout << "-----------------------------------" << std::endl;
	resCsBkg->Print();
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
