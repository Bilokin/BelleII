
#ifndef __fitFunctions_C_
#define __fitFunctions_C_
#include "fitSettings.C"
#ifndef __MYROOFIT__
#define __MYROOFIT__

#include "RooGlobalFunc.h"
#include "RooRealVar.h"
#include "RooConstVar.h"
#include "RooBCPEffDecay.h"
#include "RooBDecay.h"
#include "RooFormulaVar.h"
#include "RooTruthModel.h"
#include "RtypesCore.h"
#include "RooCategory.h"
using namespace RooFit ;
#endif

RooResolutionModel * getDeltaTResolution(fitSettings & set, RooRealVar & dt)
{
	if (set.fres.size() < 2 || set.sigmares.size() < 2) 
	{
		std::cout << "Error in getDeltaTResolution: Input parameters not set!" << std::endl;
		return NULL;
	}
	if (set.useDeltaResolution) 
	{
		return new RooTruthModel("tm","truth model",dt);
	}
	RooRealVar* mgSigB = new RooRealVar("mgSigB","mg",0);
	
	RooRealVar* fres1 = new RooRealVar("fres1","fres parameter",set.fres[0]);
	RooRealVar* fres2 = new RooRealVar("fres2","fres parameter",set.fres[1]);
	RooRealVar *fres3 = new RooRealVar("fres3","fres parameter",set.fres[2]);

	RooRealVar* sgSigB1 = new RooRealVar("sgSigB1","sg",set.sigmares[0]);
	RooRealVar* sgSigB2 = new RooRealVar("sgSigB2","sg",set.sigmares[1]);
	RooRealVar* sgSigB3 = new RooRealVar("sgSigB3","sg",set.sigmares[2]);
	RooGaussModel* gaussSigB1 = new RooGaussModel("gaussSigB1","gauss", dt, *mgSigB, *sgSigB1);
	RooGaussModel* gaussSigB2 = new RooGaussModel("gaussSigB2","gauss", dt, *mgSigB, *sgSigB2);
	RooGaussModel* gaussSigB3 = new RooGaussModel("gaussSigB3","gauss", dt, *mgSigB, *sgSigB3);
	
	RooAddModel* combinedSignalRes = new RooAddModel("combinedSignalRes","combinedSignalRes", RooArgList(*gaussSigB1,*gaussSigB2, *gaussSigB3), RooArgList(*fres1, *fres2,*fres3));
	//combinedSignalRes.fixCoefNormalization(RooArgList(fres1, fres2,fres3));
	return combinedSignalRes;
}

RooAbsPdf * getDeltaTBkg(fitSettings & set, RooRealVar & dt, bool fixParameters = false)
{
	if (set.fbkg.size() < 1 || set.sigmabkg.size() < 2) 
	{
		std::cout << "Error in getDeltaTBkg: Input parameters not set!" << std::endl;
		return NULL;
	}
	RooRealVar* mgBkg = new RooRealVar("mgBkg","mg2",0);
	RooRealVar* fbkg1 = new RooRealVar("fbkg1","fbkg parameter",set.fbkg[0],0,1);
	RooRealVar* sgBkg1 = new RooRealVar("sgBkg1","sg2",set.sigmabkg[0],0,15);
	RooRealVar* sgBkg2 = new RooRealVar("sgBkg2","sg2",set.sigmabkg[1],0.,15);
	if (fixParameters) 
	{
		fbkg1->setConstant();
		sgBkg1->setConstant();
		sgBkg2->setConstant();
	}
	RooGaussian* gaussBkg1 = new RooGaussian("gaussBkg1","gauss",dt,*mgBkg,*sgBkg1);
	RooGaussian* gaussBkg2 = new RooGaussian("gaussBkg2","gauss",dt,*mgBkg,*sgBkg2);
	
	RooAddPdf * combinedBkg = new RooAddPdf("combinedBkg","combinedBkg", *(new RooArgList(*gaussBkg2,*gaussBkg1)), *(new RooArgList(*fbkg1)));

	return combinedBkg;
}
RooAbsPdf * getDeltaTSignal(fitSettings & set, RooRealVar & dt, RooCategory & q, RooRealVar &w, RooRealVar &A, RooRealVar &S)
{
	RooRealVar* dm = new RooRealVar("dm","delta m(B0)",0.509);
	RooRealVar* tau = new RooRealVar("tau","tau (B0)",1.52);
	RooRealVar* dw = new RooRealVar("dw","delta mistag rate for B0/B0bar",set.dw);
	
	RooRealVar* CPeigen = new RooRealVar("CPeigen","CP eigen value",1);
	RooRealVar* effR = new RooRealVar("effR","B0/B0bar reco efficiency ratio",0.) ;

	RooResolutionModel * combinedSignalRes = getDeltaTResolution(set, dt);
	RooBCPGenDecay * bcp = new RooBCPGenDecay("bcp","bcp", dt, q, *tau, *dm, w, A, S, *dw, *effR, *combinedSignalRes, RooBCPGenDecay::DoubleSided) ;
	return bcp;
}

RooAbsPdf * getDeltaTCombined(fitSettings & set, RooRealVar & dt, RooCategory & q, RooRealVar &w, RooRealVar &A, RooRealVar &S)
{
	RooRealVar* fsig1  = new RooRealVar("fsig1","fsig parameter",set.fsig);
	RooRealVar* fsig2 = new RooRealVar("fsig2","fsig parameter",1-set.fsig);

	RooAbsPdf * bcp = getDeltaTSignal(set,dt,q,w,A,S);
	RooAbsPdf * combinedBkg = getDeltaTBkg(set,dt);
	
	RooAddPdf * combinedQ = new RooAddPdf("combinedQ","combinedQ", RooArgList(*bcp,*combinedBkg), RooArgList(*fsig1));
	return combinedQ;
}


RooAbsPdf * getMbcSignal(fitSettings & set, RooRealVar & mbc)
{
	if (set.mbcSigPar.size() < 3 ) 
	{
		std::cout << "Error in getMbcSignal: Input parameters not set!" << std::endl;
		return NULL;
	}
// --- Build Mbc CB signal PDF ---
	RooRealVar*  sigmean = new RooRealVar("sigmean","B^{#pm} mass",set.mbcSigPar[0]);//, 5.2, 5.3) ;
	RooRealVar * sigsigma = new RooRealVar("sigsigma","B^{#pm} width",set.mbcSigPar[1]);//,0.000001,1.) ;
	RooRealVar * sigalpha = new RooRealVar("sigalpha","B^{#pm} width",set.mbcSigPar[2]);//,1,4.) ;
	RooRealVar * sign = new RooRealVar("sign","B^{#pm} width", set.mbcSigPar[3]);//,1,20.) ;
	RooCBShape * CBall = new RooCBShape("CBall", "Crystal Ball shape", mbc, *sigmean, *sigsigma, *sigalpha, *sign);
	return CBall;
}

RooAbsPdf * getMbcBkg(fitSettings & set, RooRealVar & mbc, bool fixParameters = false)
{
	if (set.mbcBkgPar.size() < 1) 
	{
		std::cout << "Error in getMbcBkg: Input parameters not set!" << std::endl;
		return NULL;
	}
	RooRealVar * argpar = new RooRealVar("argpar","argus shape parameter",set.mbcBkgPar[1],-200.,-1.) ;
	if (fixParameters) 
	{
		argpar->setConstant();
	}
	RooArgusBG * argus = new RooArgusBG("argus","Argus PDF",mbc,RooConst(set.mbcBkgPar[0]),*argpar) ;
	return argus;	
}

RooAbsPdf * getCsSignal(fitSettings & set, RooRealVar & cs)
{
	if (set.csSigPar.size() < 5 ) 
	{
		std::cout << "Error in getCsSignal: Input parameters not set!" << std::endl;
		return NULL;
	}
	RooRealVar * csmean1 = new RooRealVar("csmean1","CS mean 1", set.csSigPar[1]) ;
	RooRealVar * cssigma1 = new RooRealVar("cssigma1","CS sigma 1",set.csSigPar[3]) ;
	RooRealVar * csmean2 = new RooRealVar("csmean2","CS mean 2",set.csSigPar[2]) ;
	RooRealVar * cssigma2 = new RooRealVar("cssigma2","CS sigma 2",set.csSigPar[4]) ;
	
	RooRealVar * fcssig = new RooRealVar("fcssig","Gaussian proportion",set.csSigPar[0]) ;
        RooGaussian * gaussCs1 = new RooGaussian("gaussCs1","gaussian PDF",cs,*csmean1,*cssigma1) ;
        RooGaussian * gaussCs2 = new RooGaussian("gaussCs2","gaussian PDF",cs,*csmean2,*cssigma2) ;
	RooAddPdf * csCBall = new RooAddPdf("csCBall","g+a",RooArgList(*gaussCs1,*gaussCs2),RooArgList(*fcssig)) ;
	return csCBall;
}
RooAbsPdf * getCsBkg(fitSettings & set, RooRealVar & cs, bool fixParameters = false)
{
	if (set.csBkgPar.size() < 1) 
	{
		std::cout << "Error in getCsBkg: Input parameters not set!" << std::endl;
		return NULL;
	}
	RooRealVar * csbkgmean1 = new RooRealVar("csbkgmean1","CS bkg mean",set.csBkgPar[0], -4., 0.) ;
	RooRealVar * csbkgsigma1 = new RooRealVar("csbkgsigma1","CS bkg sigma ",set.csBkgPar[1],0.001,10.) ;
        RooGaussian * gaussCsBkg1 = new RooGaussian("gaussCsBkg1","gaussian PDF",cs,*csbkgmean1,*csbkgsigma1) ;
	
	
	//RooRealVar * csbkgpar = new RooRealVar("csbkgpar","B^{#pm} mass",set.csBkgPar[0] -30, 30) ;
	if (fixParameters) 
	{
		//csbkgpar->setConstant();
		csbkgmean1->setConstant();
		csbkgsigma1->setConstant();
	}
	//RooExponential* csbkg = new RooExponential("csbkg", "csbkg", cs, *csbkgpar);
	return gaussCsBkg1;
}

RooAbsPdf * getMbcCombined(fitSettings & set, RooRealVar & mbc, RooRealVar& fbkg)
{
	RooAbsPdf * CBall = getMbcSignal(set,mbc);
// --- Build Argus background PDF ---
	RooAbsPdf* argus = getMbcBkg(set, mbc);
// --- Build signal+ background PDF ---
	RooArgList * mbclist = new RooArgList(*CBall,*argus);
	RooAddPdf * mbcsum = new RooAddPdf("mbcsum","g+a mbc", *mbclist,RooArgList(fbkg)) ;
	return mbcsum;
}
RooAbsPdf * getDeBkg(fitSettings & set,  RooRealVar & de, bool fixParameters = false)
{
	if (set.deBkgPar.size() < 2) 
	{
		std::cout << "Error in getDeBkg: Input parameters not set!" << std::endl;
		return NULL;
	}
	RooRealVar * c0 = new RooRealVar("c0","coefficient #0", set.deBkgPar[0],-1.,1.) ;
	RooRealVar * c1 = new RooRealVar("c1","coefficient #1", set.deBkgPar[1],-1.,1.) ;
	RooRealVar * c2 = new RooRealVar("c2","coefficient #2", set.deBkgPar[2],-1.,1.) ;
	if (fixParameters) 
	{
		c0->setConstant();
		c1->setConstant();
		c2->setConstant();
	}
	RooChebychev * chebychev = new RooChebychev("chebychev","background p.d.f.",de,RooArgList(*c0,*c1,*c2)) ; 
	return chebychev;	
}
RooAbsPdf * getDeSignal(fitSettings & set,  RooRealVar & de)
{
	if (set.deSigPar.size() < 3) 
	{
		std::cout << "Error in getDeSignal: Input parameters not set!" << std::endl;
		return NULL;
	}
// --- Build deltaE CB signal PDF ---
	RooRealVar * demean = new RooRealVar("demean","B^{#pm} mass", set.deSigPar[0]) ;
	RooRealVar * desigma = new RooRealVar("desigma","B^{#pm} width",set.deSigPar[1]);//,0.001,1.) ;
	RooRealVar * dealpha = new RooRealVar("dealpha","B^{#pm} width", set.deSigPar[2]);//,0,2.) ;
	RooRealVar * den = new RooRealVar("den","B^{#pm} width", set.deSigPar[3]);//,0.5,20.) ;
	RooCBShape * deCBall = new RooCBShape("deCBall", "Crystal Ball shape", de, *demean, *desigma, *dealpha, *den);
	return deCBall;
}
#endif
