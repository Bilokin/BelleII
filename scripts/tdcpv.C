#include "fitFunctions.C"
#include "convolution3.C"
#include "mbcdedtFit.C"
#include "deltaT.C"

#ifndef	__MYROOFIT__
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

void addBranches(TTree* T, fitSettings set)
{
	float dt = 0;
	float de = 0;
	float mbc = 0;
	float dterr = 0;
	int q = 1;
	float w = 0;
	float fann = 0;
	TBranch *bde = T->Branch("de",&de,"de/F");
	TBranch *bmbc = T->Branch("mbc",&mbc,"mbc/F");
	TBranch *bdt = T->Branch("dt",&dt,"dt/F");
	TBranch *bdterr = T->Branch("dterr",&dterr,"dterr/F");
	TBranch *bq = T->Branch("q",&q,"q/I");
	TBranch *bw = T->Branch("w",&w,"w/F");
	T->SetBranchAddress("B0_FANN_qrCombined",&fann);
	T->SetBranchAddress("B0_DeltaT",&dt);
	T->SetBranchAddress("B0_DeltaTErr",&dterr);
	T->SetBranchAddress("B0_deltae",&de);
	T->SetBranchAddress("B0_mbc",&mbc);
	Long64_t nentries = T->GetEntries();
	float A = 0.5;
	float S = 0.5;
	float dM = 0.51;
	float tau = 1.58;
	for (Long64_t i=0; i < nentries; i++) 
	{
		T->GetEntry(i);
		q = TMath::Sign(1,fann);
		w = (set.wparameters[0]+set.wparameters[1]*abs(fann));
		
		//std::cout << "w: " << w << std::endl;
		bdt->Fill();
		bde->Fill();
		bmbc->Fill();
		bdterr->Fill();
		bq->Fill();
		bw->Fill();
	}
	T->Write();
	//T->Print();
}

void tdcpv(string filename = "merged-xsd2/lumi555fb-merged.root", bool fullFit = false, string Kres = "Xsd", string signalname = "merged-xsd2/lumi555fb-merged.root")
{
	string outputfilename = "tmp-branch.root";
	string cut = getCuts(1, Kres);
	
	if (fullFit) 
	{
		cut = getBasicCuts(1,Kres);
		signalname = "signal-xsd.root";
	}
	fitSettings settings = deltaT(signalname, Kres, cut);
	TFile * file = TFile::Open(filename.c_str());
	TTree* B0Signal = (TTree*)file->Get("B0Signal");
	TFile * f2 = new TFile(outputfilename.c_str(),"recreate");
	TTree* B0Signal2 = B0Signal->CopyTree(cut.c_str());
	addBranches(B0Signal2, settings);
	B0Signal2->AutoSave();
	f2->Write();
	f2->Close();
	file->Close();
	int nbins = 100;
	//f2->cd();
	TFile * fileoutput = TFile::Open(outputfilename.c_str());
	TTree* B0Signal3 = (TTree*)fileoutput->Get("B0Signal");
	if (fullFit) 
	{
		fitSettings fullsettings = getStdSettings();
		fullsettings.sigmares = settings.sigmares;
		fullsettings.fres = settings.fres;
		mbcdedtFit(B0Signal3, fullsettings);
	}
	else 
	{
		convolution(settings, B0Signal3);
	}
	//convolution(createRooHist(deltaTBHist), createRooHist(deltaTBbarHist));
}
