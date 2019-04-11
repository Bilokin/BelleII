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
void massfit(string filename = "official-skims/hlt_hadron-366ipb-cutsync-PROD4.root", bool showfit = true)
{
	float dstrange[2] = {1.6,2.4};
	RooRealVar meta("meta","M_{#eta} [GeV]",0.45,0.6) ;
	RooRealVar mpi0("mpi0","M_{#pi_{0}} [GeV]",0.09,0.17) ;
	RooRealVar mdst("mdst","M_{D*} [GeV]",dstrange[0],dstrange[1]) ;
	//RooRealVar mdst("mdst","m_{K^{0}_{S}} [GeV]",dstrange[0],dstrange[1]) ;
	TFile * file = TFile::Open(filename.c_str());
	TTree* etaSignal = (TTree*)file->Get("etas");
	TTree* pi0Signal = (TTree*)file->Get("pi0s");
	TTree* dstSignal = (TTree*)file->Get("D0signal");
	//TTree* dstSignal = (TTree*)file->Get("ks0");
	string etacut = getCuts("eta");
	string pi0cut = getCuts("pi0");
	string dstcut = "";//getCuts("DST");
	//string dstcut = getCuts("K_S0");
	TH1F * mpi0Hist = new TH1F("mpi0Hist", ";M [GeV]", 50,0.09,0.17);
	TH1F * metaHist = new TH1F("metaHist", ";M [GeV]", 30,0.45,0.6);
	TH1F * mdstHist = new TH1F("mdstHist", ";M [GeV]", 50,dstrange[0],dstrange[1]);
	float npi0s = pi0Signal->Project("mpi0Hist","pi0_M",(pi0cut).c_str());
	float netas = etaSignal->Project("metaHist","eta_M",(etacut).c_str());
	float ndsts = dstSignal->Project("mdstHist","DST_M",(dstcut).c_str());
	//float ndsts = dstSignal->Project("mdstHist","K_S0_M",(dstcut).c_str());
	RooDataHist metaRooHist("metaRooHist","metaRooHist",meta,Import(*metaHist)) ;
	RooDataHist mpi0RooHist("mpi0RooHist","mpi0RooHist",mpi0,Import(*mpi0Hist)) ;
	RooDataHist mdstRooHist("mdstRooHist","mdstRooHist",mdst,Import(*mdstHist)) ;

	RooRealVar fpsig("fpsig","B^{#pm} mass",0.1, 0, 1) ;
	RooRealVar fesig("fesig","B^{#pm} mass",0.01, 0, 1) ;
	RooRealVar fdstig("fdstig","B^{#pm} mass",0.001, 0, 1) ;

	RooRealVar pi0mean("pi0mean","B^{#pm} mass", 0.09, 0.17) ;
	RooRealVar dstmean("dstmean","B^{#pm} mass", 1.8, 2.2) ;
	//RooRealVar dstmean("dstmean","B^{#pm} mass", 0.35, 0.7) ;
	RooRealVar etamean("etamean","B^{#pm} mass", 0.45, 0.6) ;
	RooRealVar pi0sigma("pi0sigma","B^{#pm} width",0.0027,0.000001,1.) ;
	RooRealVar dstsigma("dstsigma","B^{#pm} width",0.027,0.01,1.) ;
	RooRealVar etasigma("etasigma","B^{#pm} width",0.01,0.005,1.) ;
	RooRealVar pi0alpha("pi0alpha","B^{#pm} width",0.1,4.) ;
	RooRealVar etaalpha("etaalpha","B^{#pm} width",0.01,20.) ;
	RooRealVar dstalpha("dstalpha","B^{#pm} width",4);//,0.1,4.) ;
	RooRealVar pi0n("pi0n","B^{#pm} width",1,20.) ;
	RooRealVar etan("etan","B^{#pm} width",20.) ;
	RooRealVar dstn("dstn","B^{#pm} width",20.);//,1,200.) ;
	RooCBShape CBallpi0("CBallpi0", "Crystal Ball shape", mpi0, pi0mean, pi0sigma, pi0alpha, pi0n);
	RooCBShape CBalleta("CBalleta", "Crystal Ball shape", meta, etamean, etasigma, etaalpha, etan);
	RooCBShape CBalldst("CBalldst", "Crystal Ball shape", mdst, dstmean, dstsigma, dstalpha, dstn);

// --- Build Chebychev background PDF ---
	RooRealVar pc0("pc0","coefficient #0", 1.0,-1000.,1000.) ;
	RooRealVar pc1("pc1","coefficient #1", 0.1,-1000.,1000.) ;
	RooRealVar pc2("pc2","coefficient #1", 0.1,-1000.,1000.) ;
	RooRealVar ec0("ec0","coefficient #0", 1.0,-1000.,1000.) ;
	RooRealVar ec1("ec1","coefficient #1", 0.1,-1000.,1000.) ;
	RooRealVar ec2("ec2","coefficient #1", 0.1,-1000.,1000.) ;
	RooRealVar kc0("kc0","coefficient #0", 1.0,-100.,100.) ;
	RooRealVar kc1("kc1","coefficient #1", 0.1,-100.,100.) ;
	RooRealVar kc2("kc2","coefficient #1", 0.1,-1000.,1000.) ;
	RooPolynomial polpi0("polpi0","background p.d.f.",mpi0,RooArgList(pc0,pc1)) ; 
	RooPolynomial poleta("poleta","background p.d.f.",meta,RooArgList(ec0,ec1)) ; 
	RooPolynomial poldst("poldst","background p.d.f.",mdst,RooArgList(kc0,kc1,kc2)) ; 
	//RooChebychev poldst("poldst","background p.d.f.",mdst,RooArgList(kc0,kc1)) ; 

	
	RooAddPdf totalpi0("totalpi0","g+a",RooArgList(CBallpi0,polpi0),RooArgList(fpsig)) ;
	RooAddPdf totaleta("totaleta","g+a",RooArgList(CBalleta,poleta),RooArgList(fesig)) ;
	RooAddPdf totaldst("totaldst","g+a",RooArgList(CBalldst,poldst),RooArgList(fdstig)) ;
	
	RooFitResult* resMeta = totaleta.fitTo(metaRooHist,Save());
	//sigmean.setRange(0.09,0.17);
	RooFitResult* resMpi0 = totalpi0.fitTo(mpi0RooHist,Save());
	RooFitResult* resMdst = totaldst.fitTo(mdstRooHist,Save());

	TCanvas* c = new TCanvas("bphysics","bphysics",400,400) ;
	//c->Divide(2,1);
	//c->cd(1);
	RooPlot* mpi0frame = mpi0.frame() ;
	mpi0frame->SetTitle("");
	mpi0RooHist.plotOn(mpi0frame) ;
	if (showfit) 
	{
		totalpi0.plotOn(mpi0frame) ;
		//totalpi0.plotOn(mpi0frame, Components(polpi0),LineStyle(kDashed), LineColor(kGray), FillStyle(3004), FillColor(kGray), DrawOption("FB")) ;
		totalpi0.plotOn(mpi0frame, Components(polpi0),LineStyle(kDashed), LineColor(kGray)) ;
		//totalpi0.plotOn(mpi0frame, Components(CBallpi0),LineStyle(kDashed), LineColor(kBlue), FillStyle(3005), FillColor(kBlue), DrawOption("FB")) ;
		//totalpi0.plotOn(mpi0frame, Components(CBallpi0),LineStyle(kDashed), LineColor(kBlue)) ;
	}
	//mpi0RooHist.plotOn(mpi0frame) ;
	//gPad->SetLeftMargin(0.18) ;gPad->SetRightMargin(0.025) ; mpi0frame->GetYaxis()->SetTitleOffset(1.8) ; mpi0frame->Draw() ;
	//c->cd(2);
	RooPlot* metaframe = meta.frame() ;
	metaframe->SetTitle("");
	metaRooHist.plotOn(metaframe) ;
	if (showfit) 
	{
		totaleta.plotOn(metaframe) ;
		//totaleta.plotOn(metaframe, Components(poleta),LineStyle(kDashed), LineColor(kGray), FillStyle(3004), FillColor(kGray), DrawOption("FB") ) ;
		totaleta.plotOn(metaframe, Components(poleta),LineStyle(kDashed), LineColor(kGray)) ;
		//totaleta.plotOn(metaframe, Components(CBalleta),LineStyle(kDashed), LineColor(kBlue), FillStyle(3005), FillColor(kBlue), DrawOption("FB") ) ;
		//totaleta.plotOn(metaframe, Components(CBalleta),LineStyle(kDashed), LineColor(kBlue)) ;
	}
	//metaRooHist.plotOn(metaframe) ;
	//gPad->SetLeftMargin(0.18) ;gPad->SetRightMargin(0.025) ; metaframe->GetYaxis()->SetTitleOffset(1.8) ; metaframe->Draw() ;
	c->cd(1);
	RooPlot* mdstframe = mdst.frame() ;
	mdstframe->SetTitle("");
	mdstRooHist.plotOn(mdstframe) ;
	if (showfit) 
	{
		totaldst.plotOn(mdstframe) ;
		//totaldst.plotOn(mdstframe, Components(poldst),LineStyle(kDashed), LineColor(kGray), FillStyle(3004), FillColor(kGray), DrawOption("FB")) ;
		totaldst.plotOn(mdstframe, Components(poldst),LineStyle(kDashed), LineColor(kGray)) ;
		//totaldst.plotOn(mdstframe, Components(CBalldst),LineStyle(kDashed), LineColor(kBlue), FillStyle(3005), FillColor(kBlue), DrawOption("FB")) ;
		//totaldst.plotOn(mdstframe, Components(CBalldst),LineStyle(kDashed), LineColor(kBlue)) ;
	}
	mdstRooHist.plotOn(mdstframe) ;
	gPad->SetLeftMargin(0.15) ; mdstframe->GetYaxis()->SetTitleOffset(1.6) ; mdstframe->Draw() ;
	myText(       0.3,  0.55, 1, "Y(4S)");
	myText(       0.6, 0.25, 1, "#int L dt ~ 389 pb^{-1}");
	myMarkerText( 0.35, 0.27, 1, 20, "Data 2018",1.3);
	std::cout << "-----------------------------------" << std::endl;
	std::cout << "N pi0: " << npi0s << "; predicted value: " << npi0s * fpsig.getVal() << " N sigmas: " << npi0s * fpsig.getVal() / sqrt(npi0s) << std::endl;
	resMpi0->Print();
	std::cout << "-----------------------------------" << std::endl;
	std::cout << "N eta: " << netas << "; predicted value: " << netas * fesig.getVal() << " N sigmas: " << netas * fesig.getVal() / sqrt(netas)  << std::endl;
	resMeta->Print();
	std::cout << "-----------------------------------" << std::endl;
	std::cout << "N dst: " << ndsts << "; predicted value: " << ndsts * fdstig.getVal() << " N sigmas: " << ndsts * fdstig.getVal() / sqrt(ndsts)  << std::endl;
	resMdst->Print();
	c->SaveAs("pdf/dst-mass-prod4-note.pdf");
}
