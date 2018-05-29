
#include "TH1F.h"
#include "TPad.h"
#include "TRandom.h"

#include "Belle2Labels.h"
#include "Belle2Style.h"

void BasicExample()
{
  SetBelle2Style();

  // generate some random data
  TH1F* hpx  = new TH1F("hpx","This is the px distribution",100,-4,4);
  gRandom->SetSeed();
  for (Int_t i = 0; i < 25000; i++) {
    hpx->Fill(gRandom->Gaus());
  }
  hpx->GetXaxis()->SetTitle("random variable [unit]");
  hpx->GetYaxis()->SetTitle("#frac{dN}{dr} [unit^{-1}]");
  hpx->SetMaximum(1000.);

  // and plot it
  hpx->Draw();

#ifdef __CINT__
  gROOT->LoadMacro("Belle2Labels.C");
#endif

  BELLE2Version("1.0000");

  return;
}

#ifndef __CINT__
int main() { 
  BasicExample();
  gPad->Print("basic.png");
  return 0;
}
#endif
