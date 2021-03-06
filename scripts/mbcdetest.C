
#include "../../roofit/fitFunctions.C"
#include "fitStdSettings.C"
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
void mbcdetest(string filename = "root/signal-mc9-afteroptimization-merged.root", string filenameBkg = "root/after-optimization-csmva-veto3-merged.root")
{
	fitSettings settings = getStdSettings();
	settings.useConstMistag = true;
	RooRealVar mbc("mbc","m_{bc} [GeV]",5.20,5.30) ;
	RooRealVar de("de","#Delta E [GeV]",-0.2,0.2) ;
	RooRealVar cs("cs","CSMVA []",-2,2.5) ;
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
	string mccut= "(B0_isSignal) &&";
	string bkgcut= "(!B0_isSignal) && B0_isContinuumEvent && abs(B0_gamma_MC_MOTHER_ID) !=511  &&";
	//string bkgcut= "B0_isContinuumEvent && ";
	string cut = getCuts("");
	string dtcut = "";//" && B0_mbc > 5.27 && B0_deltae > -0.25 && B0_deltae < 0.25"; 
	TH1F * mbcHist = new TH1F("mbcHist", ";M [GeV]", 100,5.2,5.3);
	TH1F * deHist = new TH1F("deHist", ";M [GeV]", 50,-0.2,0.2);
	TH1F * csHist = new TH1F("csHist", ";cs [GeV]", 50,-2,2.5);
	TH1F * dtHist = new TH1F("dtHist", ";dt [ps]", 50,-10,10);
	B0Signal->Project("mbcHist","B0_mbc",(mccut+cut).c_str());
	B0Signal->Project("deHist","B0_deltae",(mccut+cut).c_str());
	B0Signal->Project("csHist","B0_CSMVA",(mccut+cut).c_str());
	B0Signal->Project("dtHist","B0_DeltaT",(mccut+cut+dtcut).c_str());

	RooDataHist deRooHist("deRooHist","deRooHist",de,Import(*deHist)) ;
	RooDataHist mbcRooHist("mbcRooHist","mbcRooHist",mbc,Import(*mbcHist)) ;
	RooDataHist csRooHist("csRooHist","csRooHist",cs,Import(*csHist)) ;
	RooDataHist dtRooHist("dtRooHist","dtRooHist",dt,Import(*dtHist)) ;

	TFile * filebkg = TFile::Open(filenameBkg.c_str());
	TTree* B0SignalBkg = (TTree*)filebkg->Get("B0Signal");
	TH1F * mbcBkgHist = new TH1F("mbcBkgHist", ";M [GeV]", 50,5.2,5.3);
	TH1F * deBkgHist = new TH1F("deBkgHist", ";de [GeV]", 25,-0.2,0.2);
	TH1F * csBkgHist = new TH1F("csBkgHist", ";cs [GeV]", 50,-2,2.5);
	TH1F * dtBkgHist = new TH1F("dtBkgHist", ";dt [ps]", 50,-10,10);
	B0SignalBkg->Project("mbcBkgHist","B0_mbc",(bkgcut+cut).c_str());
	B0SignalBkg->Project("deBkgHist","B0_deltae",(bkgcut+cut).c_str());
	B0SignalBkg->Project("csBkgHist","B0_CSMVA",(bkgcut+cut).c_str());
	B0SignalBkg->Project("dtBkgHist","B0_DeltaT",(bkgcut+cut+dtcut).c_str());

	RooDataHist mbcBkgRooHist("mbcBkgRooHist","mbcBkgRooHist",mbc,Import(*mbcBkgHist)) ;
	RooDataHist deBkgRooHist("deBkgRooHist","deBkgRooHist",de,Import(*deBkgHist)) ;
	RooDataHist csBkgRooHist("csBkgRooHist","csBkgRooHist",cs,Import(*csBkgHist)) ;
	RooDataHist dtBkgRooHist("dtBkgRooHist","dtBkgRooHist",dt,Import(*dtBkgHist)) ;
	
	RooRealVar fsig1("fsig1","fsig parameter", 0.5);
// --- Build Gaussian signal PDF ---
	RooRealVar demean("demean","B^{#pm} mass",-0.01,-0.2,0.2) ;
	RooRealVar desigma("desigma","B^{#pm} width",0.04,0.001,1.) ;
	RooRealVar dealpha("dealpha","B^{#pm} width",0,2.) ;
	RooRealVar den("den","B^{#pm} width",0.5,20.) ;
	RooCBShape deCBall("deCBall", "Crystal Ball shape", de, demean, desigma, dealpha, den);
// --- Build Gaussian signal PDF ---
	RooRealVar sigmean("sigmean","B^{#pm} mass",5.2795, 5.275, 5.3) ;
	RooRealVar sigsigma("sigsigma","B^{#pm} width",0.0027,0.000001,2.) ;
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
	RooChebychev chebychev("chebychev","background p.d.f.",de,RooArgList(c0,c1)) ; 
// --- Construct signal+background PDF ---
	RooRealVar farggauss("farggauss","#signal events",0.1,0.,1) ;
	RooAddPdf sum("sum","g+a",RooArgList(gauss,argus),RooArgList(farggauss)) ;
// --- Build CS signal PDF ---
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
// --- Build CS Bkg PDF ---
	RooRealVar bbcsmean("bbcsmean","B^{#pm} mass",0.2795, 0, 2) ;
	RooRealVar bbcssigma("bbcssigma","B^{#pm} width",0.27,0.000001,10.) ;
	RooRealVar bbcsalpha("bbcsalpha","B^{#pm} width",1,10.) ;
	RooRealVar bbcsn("bbcsn","B^{#pm} width",1,40.) ;
	RooCBShape bbcsCBall("bbcsCBall", "Crystal Ball shape", cs, bbcsmean, bbcssigma, bbcsalpha, bbcsn);
	RooRealVar csbkgmean1("csbkgmean1","B^{#pm} mass",-0.7,-1.,1) ;
	RooRealVar csbkgsigma1("csbkgsigma1","B^{#pm} width",0.27,0.001,10.) ;
        RooGaussian gaussCsBkg1("gaussCsBkg1","gaussian PDF",cs,csbkgmean1,csbkgsigma1) ;
	
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
	RooFitResult* resMbcSig = CBall.fitTo(mbcRooHist,Save());
	RooFitResult* resDeSig = deCBall.fitTo(deRooHist,Save());
	RooFitResult* resMbcBkg = sum.fitTo(mbcBkgRooHist,Save());
	RooFitResult* resDeBkg = chebychev.fitTo(deBkgRooHist,Save());
	RooFitResult* resCsSig = csCBall.fitTo(csRooHist,Save());
	//RooFitResult* resCsBkg = gaussCsBkg1.fitTo(csBkgRooHist,Save());
	RooFitResult* resCsBkg = bbcsCBall.fitTo(csBkgRooHist,Save());
	RooFitResult* resDtBkg = dtbkg->fitTo(dtBkgRooHist,Save());
	RooFitResult* resDtSig = dtsignal->fitTo(dtRooHist,Save());
// --- Plot toy data and composite PDF overlaid ---
	TCanvas* c = new TCanvas("bphysics","bphysics",1500,1000) ;
	c->Divide(4,2);
	c->cd(1);
	RooPlot* mbcframe = mbc.frame() ;
	//data->plotOn(mbcframe) ;
	mbcRooHist.plotOn(mbcframe) ;
	CBall.plotOn(mbcframe) ;
	gPad->SetLeftMargin(0.15) ; mbcframe->GetYaxis()->SetTitleOffset(1.6) ; mbcframe->Draw() ;
	
	c->cd(2);
	RooPlot* mbcframe2 = mbc.frame() ;
	mbcBkgRooHist.plotOn(mbcframe2) ;
	sum.plotOn(mbcframe2) ;
	sum.plotOn(mbcframe2,Components(argus),LineStyle(kDashed)) ; 
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
	bbcsCBall.plotOn(csframe2) ;
	gPad->SetLeftMargin(0.15) ; csframe2->GetYaxis()->SetTitleOffset(1.6) ; csframe2->Draw() ;
	c->cd(7);
	RooPlot* dtframe = dt.frame() ;
	dtRooHist.plotOn(dtframe) ;
	dtsignal->plotOn(dtframe) ;
	gPad->SetLeftMargin(0.15) ; dtframe->GetYaxis()->SetTitleOffset(1.6) ; dtframe->Draw() ;
	c->cd(8);
	RooPlot* dtframe2 = dt.frame() ;
	dtBkgRooHist.plotOn(dtframe2) ;
	dtbkg->plotOn(dtframe2) ;
	gPad->SetLeftMargin(0.15) ; dtframe2->GetYaxis()->SetTitleOffset(1.6) ; dtframe2->Draw() ;
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
	std::cout << "-----------------------------------" << std::endl;
	resDtBkg->Print();
	std::cout << "-----------------------------------" << std::endl;
	std::cout << "Nevents: " << csHist->GetEntries() << std::endl;
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
