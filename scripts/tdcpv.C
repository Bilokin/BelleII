
//
//      	  
//      II 
//                                
//      II  PPPP   HH  HH    CCCC   
//      II  PP  P  HH  HH  CC 
//      II  PPPP   HH  HH  CC
//      II  PP     HHHHHH  CC
//      II  PP     HH  HH    CCCC    STRASBOURG 2017
//      
//        
//                        Author: Bilokin S.    
//

#include "../../roofit/fitFunctions.C"
#include "fitStdSettings.C"
//#include "convolution3.C"
#include "../../roofit/mbcdedtFit.C"
#include "../../roofit/deltaT.C"

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


float computeW(float fann, fitSettings set)
{
	for (unsigned int i = 0; i < set.wvalues.size(); i++) 
	{
		if (abs(fann) > set.qbins[i] && abs(fann) <  set.qbins[i+1]  ) 
		{
			return set.wvalues[i];
		}
	}
	std::cout << "Q has wrong value!!!" << std::endl;
	return -1;
}

void addBranches(TTree* T, fitSettings set)
{
	float dt = 0;
	float de = 0;
	float cs = 0;
	float mbc = 0;
	float dterr = 0;
	int q = 1;
	float w = 0;
	float fann = 0;
	TBranch *bcs = T->Branch("cs",&cs,"cs/F");
	TBranch *bde = T->Branch("de",&de,"de/F");
	TBranch *bmbc = T->Branch("mbc",&mbc,"mbc/F");
	TBranch *bdt = T->Branch("dt",&dt,"dt/F");
	TBranch *bdterr = T->Branch("dterr",&dterr,"dterr/F");
	TBranch *bq = T->Branch("q",&q,"q/I");
	TBranch *bw = T->Branch("w",&w,"w/F");
	T->SetBranchAddress("B0_FBDT_qrCombined",&fann);
	T->SetBranchAddress("B0_DeltaT",&dt);
	T->SetBranchAddress("B0_DeltaTErr",&dterr);
	T->SetBranchAddress("B0_deltae",&de);
	T->SetBranchAddress("B0_CSMVA",&cs);
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
		w = computeW(fann, set);	
		//std::cout << "w: " << w << std::endl;
		bdt->Fill();
		bde->Fill();
		bcs->Fill();
		bmbc->Fill();
		bdterr->Fill();
		bq->Fill();
		bw->Fill();
	}
	T->Write();
	//T->Print();
}

void tdcpv(string filename = "official-mc101/all--merged-1iab-kspipigamma-mc10.root", string signalname = "official-mc10/signal-merged-kpipigamma-mc10.root", bool fullFit = true, string Kres = "") //"root/signalx-veto3-precut3.root"
{
	string outputfilename = "root/tmp-branch.root";
	string cut = getCuts(Kres);
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
		fullsettings.wvalues = settings.wvalues;
		fullsettings.dw = settings.dw;
		mbcdedtFit(B0Signal3, fullsettings);
	}
	else 
	{
		//convolution(settings, B0Signal3);
	}
	//convolution(createRooHist(deltaTBHist), createRooHist(deltaTBbarHist));
}
