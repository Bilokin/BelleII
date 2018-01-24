
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

RooAbsPdf * getDeltaTBkg(fitSettings & set, RooRealVar & dt)
{
	RooRealVar* fbkg1 = new RooRealVar("fbkg1","fbkg parameter",set.fbkg[0]);
	RooRealVar* fbkg2 = new RooRealVar("fbkg2","fbkg parameter",set.fbkg[1]);
	RooRealVar* mgBkg = new RooRealVar("mgBkg","mg2",0);
	RooRealVar* sgBkg1 = new RooRealVar("sgBkg1","sg2",set.sigmabkg[0]);
	RooRealVar* sgBkg2 = new RooRealVar("sgBkg2","sg2",set.sigmabkg[1]);
	RooGaussian* gaussBkg1 = new RooGaussian("gaussBkg1","gauss",dt,*mgBkg,*sgBkg1);
	RooGaussian* gaussBkg2 = new RooGaussian("gaussBkg2","gauss",dt,*mgBkg,*sgBkg2);
	
	RooAddPdf* combinedBkg = new RooAddPdf("combinedBkg","combinedBkg", RooArgList(*gaussBkg2,*gaussBkg1), RooArgList(*fbkg2, *fbkg1));

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
// --- Build Mbc CB signal PDF ---
	RooRealVar*  sigmean = new RooRealVar("sigmean","B^{#pm} mass",set.mbcSigPar[0]);//, 5.2, 5.3) ;
	RooRealVar * sigsigma = new RooRealVar("sigsigma","B^{#pm} width",set.mbcSigPar[1]);//,0.000001,1.) ;
	RooRealVar * sigalpha = new RooRealVar("sigalpha","B^{#pm} width",set.mbcSigPar[2]);//,1,4.) ;
	RooRealVar * sign = new RooRealVar("sign","B^{#pm} width", set.mbcSigPar[3]);//,1,20.) ;
	RooCBShape * CBall = new RooCBShape("CBall", "Crystal Ball shape", mbc, *sigmean, *sigsigma, *sigalpha, *sign);
	return CBall;
}

RooAbsPdf * getMbcCombined(fitSettings & set, RooRealVar & mbc, RooRealVar& fbkg)
{
	RooAbsPdf * CBall = getMbcSignal(set,mbc);
// --- Build Argus background PDF ---
	RooRealVar * argpar = new RooRealVar("argpar","argus shape parameter",-30.0,-100.,-1.) ;
	RooArgusBG * argus = new RooArgusBG("argus","Argus PDF",mbc,RooConst(set.mbcBkgPar[0]),*argpar) ;
// --- Build signal+ background PDF ---
	RooArgList * mbclist = new RooArgList(*CBall,*argus);
	RooAddPdf * mbcsum = new RooAddPdf("mbcsum","g+a mbc", *mbclist,RooArgList(fbkg)) ;
	return mbcsum;
}
RooAbsPdf * getDeSignal(fitSettings & set,  RooRealVar & de)
{
// --- Build deltaE CB signal PDF ---
	RooRealVar * demean = new RooRealVar("demean","B^{#pm} mass", set.deSigPar[0]) ;
	RooRealVar * desigma = new RooRealVar("desigma","B^{#pm} width",set.deSigPar[1]);//,0.001,1.) ;
	RooRealVar * dealpha = new RooRealVar("dealpha","B^{#pm} width", set.deSigPar[2]);//,0,2.) ;
	RooRealVar * den = new RooRealVar("den","B^{#pm} width", set.deSigPar[3]);//,0.5,20.) ;
	RooCBShape * deCBall = new RooCBShape("deCBall", "Crystal Ball shape", de, *demean, *desigma, *dealpha, *den);
	return deCBall;
}
RooAbsPdf * getDeCombined(fitSettings & set,  RooRealVar & de, RooRealVar& fbkg)
{
	RooAbsPdf * deCBall = getDeSignal(set,de);
// --- Build Chebychev background PDF ---
	RooRealVar * c0 = new RooRealVar("c0","coefficient #0", set.deBkgPar[0],-1.,1.) ;
	RooRealVar * c1 = new RooRealVar("c1","coefficient #1", set.deBkgPar[1],-1.,1.) ;
	RooRealVar * c2 = new RooRealVar("c2","coefficient #2", set.deBkgPar[2],-1.,1.) ;
	RooChebychev * chebychev = new RooChebychev("chebychev","background p.d.f.",de,RooArgList(*c0,*c1,*c2)) ; 

// --- Build signal+ background PDF ---
	RooArgList * delist = new RooArgList(*deCBall,*chebychev);
	RooAddPdf * desum = new RooAddPdf("desum","g+a de", *delist,RooArgList(fbkg)) ;
	return desum;
}
RooAbsPdf * getMbcDe(fitSettings & set, RooRealVar & mbc, RooRealVar & de, RooRealVar & fbkg)
{
	//RooRealVar* nsig = new RooRealVar("nsig","#signal events",100,0.,10000) ;
	//RooRealVar* nbkg = new RooRealVar("nbkg","#background events",800,0.,10000);
	RooAbsPdf * mbcsum = getMbcCombined(set, mbc, fbkg);
	RooAbsPdf * desum = getDeCombined(set, de, fbkg);
// --- Build Product PDF ---
	RooProdPdf * result = new RooProdPdf("result","de*mbc",*mbcsum, *desum);
	return result;
}
#endif
