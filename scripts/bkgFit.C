


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
#include "TCanvas.h"
#include "TAxis.h"
#include "RooPlot.h"
using namespace RooFit ;

void bkgFit(TTree* tree = NULL, float fsignorm = 0.7, float wnorm = 0.21, float dwnorm = 0.)
{
	std::cout << " ____________________________" << std::endl;
	std::cout << "|                            |" << std::endl;
	std::cout << "|     BKG FIT version 0.2    |" << std::endl;
	std::cout << "|____________________________|\n" << std::endl;

	
	RooRealVar dE("dE","#Delta E [GeV]",-0.2,0.2);
	RooRealVar M("M","#M_{bc} [GeV]",5.2,5.3);
	RooRealVar Msig("M","#M_{bc} [GeV]",5.2,5.3);
	RooRealVar fsig("fsig","fsig parameter",fsignorm);

	RooRealVar m0("m0","m0",5.291,5.20,5.30) ;
	RooRealVar k("k","k",-30,-50,-10) ;

	RooArgusBG argus("argus","argus",m,m0,k);
	RooGaussian gaus("gaus","gaus",m,m0,k);
