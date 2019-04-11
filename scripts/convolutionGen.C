#include "../../roofit/fitFunctions.C"
#include "../../roofit/fitSettings.C"
#ifndef	__MYROOFIT__
#define __MYROOFIT__
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
#endif

void convolution(fitSettings set, TTree* tree = NULL, bool showSecCanvas = true)
{
	std::cout << " ____________________________" << std::endl;
	std::cout << "|                            |" << std::endl;
	std::cout << "|   FINAL FIT version 0.3    |" << std::endl;
	std::cout << "|____________________________|\n" << std::endl;
	set.Print();
	std::cout << "____________________________\n" << std::endl;
	string name = "MC";
	// Parameters
	RooRealVar dt("B0_TruthDeltaT","#Delta t [ps]",-20,20) ;
	//RooCategory q("B0_qrMC","Flavour of the tagged B0");
	//q.defineType("B",    1);
	//q.defineType("Bbar",-1);
	RooRealVar q("B0_qrMC","flavour mistag rate",-2,2);
	//RooRealVar w("w","flavour mistag rate",set.w);
	RooRealVar w("w","flavour mistag rate",0);
	RooRealVar A("A","A",0.,-1.5,1.5);
	RooRealVar S("S","S",0.5,-1.5,1.5);
	RooRealVar tauB("tauB","B lifetime",1.519);
	RooRealVar dM("dM","dM parameter",0.51);
	RooDataSet* data = NULL;
	if (tree) 
	{
		std::cout << "Importing tree: ";// << std::endl;
		data = new RooDataSet(name.c_str(), name.c_str(), RooArgSet(q,dt), Import(*tree));
		data->Print("v");
		for (unsigned int i = 0; i < 100; i++) 
		{
			data->get(i)->Print("v");
		}
		std::cout << "Done!" << std::endl;
	}
	//RooAbsPdf * bcp = getDeltaTSignal(set,dt,q,w,A,S);
	RooAbsPdf * combinedQ = new RooGenericPdf("genpdfQ","genpdfq","exp(-abs(B0_TruthDeltaT)/tauB)/(4*tauB)*(1+B0_qrMC/abs(B0_qrMC)*(A*cos(dM*B0_TruthDeltaT)+S*sin(dM*B0_TruthDeltaT)))",RooArgSet(dt,tauB,A,S,dM, q));
	combinedQ->Print();
	// Generate some data
	if (!data) 
	{
		int nevents = 14819;   // 
		w.setVal(0.232);
		w.setConstant();
		std::cout << " _________________________________________ \n" << std::endl;
		std::cout << "       Generating " << nevents << " Toy MC events" << std::endl;
		std::cout << " _________________________________________ " << std::endl;
		data = combinedQ->generate(RooArgSet(dt,q),nevents);//, ProtoData(*wdata));
		data->Print("v");
		for (unsigned int i = 0; i < 100; i++) 
		{
			data->get(i)->Print("v");
		}
	}
	RooFitResult* resb = combinedQ->fitTo(*data, Save()) ;
		std::cout << "Here!" << std::endl;
	// Plot B0 and B0bar tagged data separately
	dt.setRange(-10,10);
	RooPlot* frame = dt.frame(Title("B decay (B0/B0bar)")) ;
	
	dt.setBins(25) ;
	data->plotOn(frame);
	combinedQ->plotOn(frame, LineColor(kBlack)) ;
	data->plotOn(frame,Cut("B0_qrMC>0"),MarkerColor(kBlue), MarkerStyle(22)) ;
	//combinedQ->plotOn(frame, Cut("B0_qrMC>0"),LineStyle(kDashed), LineColor(kBlue)) ;
	data->plotOn(frame,Cut("B0_qrMC<0"),MarkerColor(kRed), MarkerStyle(23)) ;
	//combinedQ->plotOn(frame, Cut("B0_qrMC==-1"),LineStyle(kDashed), LineColor(kRed)) ;
	TCanvas* c = new TCanvas("bphysics","bphysics",1000,500) ;
	c->Divide(2,1);
	c->cd(1);
	gPad->SetLeftMargin(0.15) ; frame->GetYaxis()->SetTitleOffset(1.6) ; frame->Draw() ;
	float Arange[2] = {-0.3, 0.3};
	float Srange[2] = {-0.3, 0.3};
	RooPlot* frame2 = new RooPlot(A,S,Arange[0], Arange[1],Srange[0],Srange[1]) ;
	frame2->SetTitle("Ellipse") ;
	resb->plotOn(frame2,A,S,"MEHV")->getAttMarker("contour")->SetMarkerSize (0);
	c->cd(2);
	gPad->SetLeftMargin(0.15) ; frame2->GetYaxis()->SetTitleOffset(1.4) ; frame2->Draw() ;
	if (showSecCanvas) 
	{
		w.setBins(50) ;
		TCanvas* cw = new TCanvas("cw","w",500,500) ;
		RooPlot* framew = w.frame(Title("w (B0/B0bar)"));
		data->plotOn(framew);
		gPad->SetLeftMargin(0.15) ; framew->GetYaxis()->SetTitleOffset(1.6) ; framew->Draw() ;
	}
	std::cout << "-----------------------------" << std::endl;
	std::cout << "-----------------------------" << std::endl;
	std::cout << "-----------------------------" << std::endl;
	resb->Print();
}

void convolutionGen(string filename = "try-CPMIX/merged-signal-v6-bkg-nocharge.root", bool showSecCanvas = false)
{
	TFile * file = TFile::Open(filename.c_str());
	TTree* B0Signal = (TTree*)file->Get("B0Signal");
	TTree* B0Signal2 = B0Signal->CopyTree("B0_isSignal && abs(B0_qrMC) > 0");
	fitSettings settings;
	settings.useDeltaResolution = true;
	settings.fsig = 1;
	settings.dw = 0.0;
	
	convolution(settings, B0Signal2, showSecCanvas);
}

