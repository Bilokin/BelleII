
#include "fitSettings.C"
//#include "fitFunctions.C"
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
#include "RooFFTConvPdf.h"
//#include "contSUM.h"
//#include "contSUM.cxx"
//#include "DTbkg0.h"
//#include "DTbkg0.cxx"
using namespace RooFit ;

void mbcdetest_bkgfit2(string filename = "tmp-branch.root")
{
	fitSettings settings = getStdSettings();
	settings.useConstMistag = true;
	RooRealVar dt("dt","dt [ps]",-10,10) ;
	RooCategory q("q","Flavour of the tagged B0") ;
		q.defineType("B0",1) ;
		q.defineType("B0bar",-1) ;
	RooRealVar A("A","A",0);
	RooRealVar S("S","S",0);
	RooRealVar w("w","flavour mistag rate",0.2);
	RooRealVar dw("dw","delta mistag rate for B0/B0bar",.01);
	RooRealVar tau("tau","tau (B0)",1.547,-100,100);
	RooRealVar f1("f1","f1",1.5205e-03,0.,1.);
	RooRealVar f2("f2","f2",1.1305e-01,0.,1.);
	RooRealVar f3("f3","f3",1.0000e+00,0.,1.);
	f1.setConstant();	
	f2.setConstant();	
	f3.setConstant();	
	
	TFile * file = TFile::Open(filename.c_str());
	TTree* B0Signal = (TTree*)file->Get("B0Signal");
	string bkgcut= "(!B0_isContinuumEvent-B0_isSignal)";
	string bkgcont= "(B0_isContinuumEvent)";

	TH1F * dtBkgHist = new TH1F("dtBkgHist", ";dt [ps]", 75,-10,10);
	TH1F * dtBkgCont = new TH1F("dtBkgCont", ";dt [ps]", 100,-10,10);
	B0Signal->Project("dtBkgHist","B0_DeltaT",(bkgcut).c_str());
	B0Signal->Project("dtBkgCont","B0_DeltaT",(bkgcont).c_str());

	RooDataHist dtBkgRooHist("dtBkgRooHist","dtBkgRooHist",dt,Import(*dtBkgHist)) ;
	RooDataHist dtBkgRooCont("dtBkgRooCont","dtBkgRooCont",dt,Import(*dtBkgCont)) ;
	
	
	
	
	RooRealVar * mean1 = new RooRealVar("mean1","mean1",10, -10, 10) ;
	RooRealVar * sigma1 = new RooRealVar("sigma1","sigma1",10,0.,10.) ;
	RooGaussian * gauss1 = new RooGaussian("gauss1","gauss1",dt,*mean1,*sigma1) ;
	
	RooRealVar * mean2 = new RooRealVar("mean2","mean2",0.84, -10, 10) ;
	RooRealVar * sigma2 = new RooRealVar("sigma2","sigma2",0.152,0.,10.) ;
	RooGaussian * gauss2 = new RooGaussian("gauss2","gauss2",dt,*mean2,*sigma2) ;
	
	RooRealVar * mean3 = new RooRealVar("mean3","mean3",-0.0775, -10, 10) ;
	RooRealVar * sigma3 = new RooRealVar("sigma3","sigma3",0.0272,0.,10.) ;
	RooGaussian * gauss3 = new RooGaussian("gauss3","gauss3",dt,*mean3,*sigma3) ;
	
	mean1->setConstant();	
	mean2->setConstant();	
	mean3->setConstant();	
	
	sigma1->setConstant();
	sigma2->setConstant();
	sigma3->setConstant();
	
	RooRealVar * meanN1 = new RooRealVar("meanN1","mean1",10, -10, 10) ;
	RooRealVar * sigmaN1 = new RooRealVar("sigmaN1","sigma1",10,0.,10.) ;
	RooGaussian * gaussN1 = new RooGaussian("gaussN1","gauss1",dt,*meanN1,*sigmaN1) ;
	
	//RooResolutionModel* combinedSignalRes = getDeltaTResolution(settings,dt);	
	
	
	RooGenericPdf * dtbkg0 = new RooGenericPdf("dtbkg0","exp(-abs(dt)/tau)*((1-q*dw)*w/2 + (1+q*dw)*(1-w)/2)/(4*tau)",RooArgSet(dt,tau,q,dw,w));
	//DTbkg0 * dtbkg0 = new DTbkg0("dtbkg0","dtbkg0",dt,tau,dw,w,q);
	
	
	RooAddPdf * combinedSignalRes = new RooAddPdf("combinedSignalRes","combinedSignalRes",RooArgList(*gauss1,*gauss2,*gauss3),RooArgList(f1,f2,f3)) ;
	RooFFTConvPdf * dtbkg = new RooFFTConvPdf("dtbkg","dtbkg",dt,*dtbkg0,*combinedSignalRes);
	
	
	RooRealVar * taubkg = new RooRealVar("taubkg","taubkg",1.5867,0,2.);
	RooRealVar * fp = new RooRealVar("fp","fp",4.0564e-01,0,1);
	RooRealVar * f1cont = new RooRealVar("f1cont","f1cont",9.8133e-01,0,10);
	RooRealVar * f2cont=  new RooRealVar("f2cont","f2cont",2.1021e-03,-10,10);
	RooRealVar * mean1cont = new RooRealVar("mean1cont","mean1cont",3.2072e-02,-10,10) ;
	RooRealVar * sigma1cont = new RooRealVar("sigma1cont","sigma1cont",6.4811e-01,0,100) ;
	RooGaussian * gauss1cont = new RooGaussian("gauss1cont","gauss1cont",dt,*mean1cont,*sigma1cont) ;
	
	RooRealVar * mean2cont = new RooRealVar("mean2cont","mean2cont",4.7977e-02,-10,10) ;
	RooRealVar * sigma2cont = new RooRealVar("sigma2cont","sigma2cont",9.1648,0,100) ;
	RooGaussian * gauss2cont = new RooGaussian("gauss2cont","gauss2cont",dt,*mean2cont,*sigma2cont) ;
	fp->setConstant();	
	//taubkg->setConstant();	
	f1cont->setConstant();	
	//f2cont->setConstant();	
	mean1cont->setConstant();	
	mean2cont->setConstant();	
	sigma1cont->setConstant();
	sigma2cont->setConstant();
	
	RooAddPdf * combinedSignalRescont = new RooAddPdf("combinedSignalRescont","combinedSignalRescont",*(new RooArgList(*gauss1cont,*gauss2cont)),*(new RooArgList(*f1cont))) ;
	
	RooTruthModel * delta = new RooTruthModel("tm","truth model",dt);
	RooGenericPdf * dtbkgcont0 = new RooGenericPdf("dtbkgcont0","exp(-abs(dt)/taubkg)",*(new RooArgSet(dt,*taubkg)));
	//RooFFTConvPdf * deltaConv = new RooFFTConvPdf("dtbkgcont1","dtbkgcont",dt,*delta,*combinedSignalRescont);
	//RooFFTConvPdf * gaussConv = new RooFFTConvPdf("gausscont1","dtbkgcont",dt,*gaussN1,*combinedSignalRescont);
	RooFFTConvPdf * dtbkgcontConv = new RooFFTConvPdf("dtbkgcontConv","dtbkgcont",dt,*dtbkgcont0,*combinedSignalRescont);
	
	RooAddPdf * sumcont = new RooAddPdf("sumcont","sumcont",*(new RooArgList(*combinedSignalRescont,*dtbkgcontConv)),*(new RooArgList(*fp))) ;
	//RooAddPdf * sumcont = new RooAddPdf("sumcont","sumcont",*(new RooArgList(*gaussN1,*dtbkgcontConv)),*(new RooArgList(*fp))) ;
	//RooFFTConvPdf * dtbkgcontConv = new RooFFTConvPdf("dtbkgcontConv","dtbkgcont",dt,*sumcont,*combinedSignalRescont);
	
	
	//contSUM * sumcont = new contSUM("sumcont","sumcont",dt,delta,taubkg,fp);
	
	
	
	
	
	
	
	
	//RooFitResult* resdtbkg = dtbkg->fitTo(dtBkgRooHist,Save());
	RooFitResult* resdtbkgcont = sumcont->fitTo(dtBkgRooCont,Save());
	//RooFitResult* resdtbkgcont = dtbkgcontConv->fitTo(dtBkgRooCont,Save());
	
	
	
	
	TCanvas* c = new TCanvas("bphysics","bphysics",1000,500) ;
	c->Divide(2,1);
	c->cd(1);
	RooPlot* dtframe1 = dt.frame() ;
	dtBkgRooHist.plotOn(dtframe1) ;
	dtbkg->plotOn(dtframe1) ;
	gPad->SetLeftMargin(0.15) ; dtframe1->GetYaxis()->SetTitleOffset(1.6) ; dtframe1->Draw() ;
	c->cd(2);
	RooPlot* dtframe2 = dt.frame() ;
	dtBkgRooCont.plotOn(dtframe2) ;
	//dtbkgcontConv->plotOn(dtframe2) ;
	//dtbkgcontConv->plotOn(dtframe2,Components(*delta),LineStyle(kDashed)) ;
	sumcont->plotOn(dtframe2) ;
	sumcont->plotOn(dtframe2,Components(*combinedSignalRescont),LineStyle(kDashed)) ;
	gPad->SetLeftMargin(0.15) ; dtframe2->GetYaxis()->SetTitleOffset(1.6) ; dtframe2->Draw() ;
	
	
	std::cout << "-----------------------------------" << std::endl;
	std::cout << "-----------------------------------" << std::endl;
	std::cout << "-----------------------------------" << std::endl;
	//resdtbkg->Print();
	std::cout << "-----------------------------------" << std::endl;
	resdtbkgcont->Print();
	std::cout << "-----------------------------------" << std::endl;
	
}
