// This is the file rootlogon.C
#include "../belle-style/Belle2Style.C"
void rootlogon()
{
  // Load Belle2 style
  //gROOT->LoadMacro("Belle2Style.C");
  //gROOT->ProcessLine(".L Belle2Style.C");
  SetBelle2Style();
  //SetMyStyle();
}
void SetMyStyle()
{
   printf("Beginning of a new ROOT session\n");

   TStyle *myStyle  = new TStyle("MyStyle","My Root Styles");

   // from ROOT plain style
   myStyle->SetPalette(1);
   myStyle->SetCanvasBorderMode(0);
   myStyle->SetPadBorderMode(0);
   myStyle->SetPadColor(0);
   myStyle->SetCanvasColor(0);
   myStyle->SetTitleColor(1);
   myStyle->SetStatColor(0);
   myStyle->SetTitleSize(0.05,"xyz");
   myStyle->SetLabelSize(0.047,"xyz"); // size of axis values

   // default canvas positioning
   myStyle->SetCanvasDefX(900);
   myStyle->SetCanvasDefY(20);
   myStyle->SetCanvasDefH(550);
   myStyle->SetCanvasDefW(540);

   myStyle->SetPadBottomMargin(0.15);
   myStyle->SetPadTopMargin(0.05);
   myStyle->SetPadLeftMargin(0.15);
   myStyle->SetPadRightMargin(0.03);


   myStyle->SetPadTickX(1);
   myStyle->SetPadTickY(1);

   myStyle->SetFrameBorderMode(0);

   // US letter
   myStyle->SetPaperSize(20, 24);


    gROOT->SetStyle("MyStyle"); //uncomment to set this style
};
string getCuts( string Kres = "", bool enableVeto = true)
{
	string cosflight = "(B0_K_10_K_S0_X*B0_K_10_K_S0_P4[0]+B0_K_10_K_S0_Y*B0_K_10_K_S0_P4[1]+B0_K_10_K_S0_Z*B0_K_10_K_S0_P4[2])/B0_K_10_K_S0_P/sqrt(B0_K_10_K_S0_X*B0_K_10_K_S0_X+B0_K_10_K_S0_Y*B0_K_10_K_S0_Y+B0_K_10_K_S0_Z*B0_K_10_K_S0_Z)";
	string cut = "";
	cut += " abs(B0_FBDT_qrCombined) > 0. "; //0.1 //0.25
	cut += " && B0_m12 > 0.6 && B0_m12 < 0.9";
	cut += " && B0_DeltaTErr < 2.5 && B0_DeltaTErr > 0";
	cut += " && abs(B0_DeltaT) < 20";
	cut += " && B0_gamma_cosTheta > -0.65 && B0_gamma_cosTheta < 0.85";
	cut += " && B0_gamma_P > 1.85 && B0_gamma_P < 3.05";
	cut += " && B0_gamma_clusterE9E21 > 0.95";

	if (Kres == "")
	{	
		cut += "&& abs(B0_K_S0_M - 0.4976) < 10.011";
		//cut += " && B0_K_S0_Rho > 0.05"; //0.1
		cut += " && B0_K_S0_significanceOfDistance > 5"; // 9
		cut += " && B0_XsdM < 2. ";
		cut += " && B0_pi0_pionID > 0.2 "; // 0.3
		cut += " && B0_pi1_pionID > 0.2 ";
		//cut += " && B0_pi0_PIDe < 0.5 ";
		//cut += " && B0_pi1_PIDe < 0.5 ";
		//cut += " && B0_pi0_PIDmu < 0.98 ";
		//cut += " && B0_pi1_PIDmu < 0.98 ";
		cut += " && B0_K_S0_pi0_pionID > 0.005 ";
		cut += " && B0_K_S0_pi1_pionID > 0.005 ";
		
	}
	if (Kres == "Xsd" || Kres == "K_10")
	{	
		cut += "&& abs(B0_"+Kres+"_K_S0_M - 0.4976) < 10.011";
		//cut += " && B0_"+Kres+"_K_S0_Rho > 0.05"; //0.1
		cut += " && B0_"+Kres+"_K_S0_significanceOfDistance > 9"; // 9
		cut += " && B0_"+Kres+"_M < 2.07 ";
		cut += " && B0_"+Kres+"_pi0_PIDpi > 0.3 "; // 0.3
		cut += " && B0_"+Kres+"_pi1_PIDpi > 0.3 ";
		cut += " && B0_"+Kres+"_pi0_PIDe < 0.95 ";
		cut += " && B0_"+Kres+"_pi1_PIDe < 0.95 ";
		//cut += " && B0_"+Kres+"_pi0_PIDmu < 0.98 ";
		//cut += " && B0_"+Kres+"_pi1_PIDmu < 0.98 ";
		cut += " && B0_"+Kres+"_K_S0_pi0_PIDpi > 0.005 ";
		cut += " && B0_"+Kres+"_K_S0_pi1_PIDpi > 0.005 ";
	}
	if (enableVeto) 
	{
		//cut += " && (abs(B0_pi0veto_MVA) < 0.5 || B0_pi0veto_w == 0)";
		//cut += " && (abs(B0_eta0veto_MVA) < 0.5 || B0_eta0veto_w == 0)";
		cut += "&& B0_pi0Likeness < 0.82"; //0.82 my
		cut += "&& B0_etaLikeness < 0.945"; //0.954 my
		cut += " && B0_CSMVA > 0.0";
		//cut += " && !B0_isContinuumEvent && !B0_isSignal ";
	}
	//cut += " && "+cosflight+" > 0.995";
	//cut += "&& B0_mbc > 5.27 && B0_deltae > -0.15 && B0_deltae < 0.05"; // BEST
	cut += " && iCand == 0 ";
	return cut;
}

string getSignalCuts(bool enableVeto = true, string Kres = "Xsd")
{
	string cut = getCuts(Kres, enableVeto);
	cut += "&& B0_mbc > 5.27 && B0_deltae > -0.15 && B0_deltae < 0.1"; // BEST
	return cut;
}
void makePrettyData(TH1F* htemp, int color = kBlack)
{
	htemp->SetMarkerStyle(20);
	htemp->SetMarkerSize(1);
	htemp->SetMarkerColor(color);
	htemp->SetLineColor(color);
	htemp->SetLineWidth(1);
}

void makePretty(TH1F* htemp, int color = kBlue, int style = 3004, int fillcolor = -1)
{
	if (fillcolor == -1) 
	{
		fillcolor = color;
	}
	htemp->SetMarkerStyle(0);
	htemp->SetLineWidth(3);
	htemp->SetFillStyle(style);
	htemp->SetFillColor(fillcolor);
	htemp->SetLineColor(color);
	htemp->SetMinimum(0);
	htemp->Sumw2();
	if(color != kBlue && style == 3004)
	{
		htemp->SetFillStyle(3005);
	}
}
