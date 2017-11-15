// This is the file rootlogon.C
{
   printf("Beginning new ROOT session\n");

   TStyle *myStyle  = new TStyle("MyStyle","My Root Styles");

   // from ROOT plain style
   myStyle->SetPalette(1);
   myStyle->SetCanvasBorderMode(0);
   myStyle->SetPadBorderMode(0);
   myStyle->SetPadColor(0);
   myStyle->SetCanvasColor(0);
   myStyle->SetTitleColor(1);
   myStyle->SetStatColor(0);
   myStyle->SetTitleSize(0.045,"xyz");
   myStyle->SetLabelSize(0.045,"xyz"); // size of axis values

   // default canvas positioning
   myStyle->SetCanvasDefX(900);
   myStyle->SetCanvasDefY(20);
   myStyle->SetCanvasDefH(550);
   myStyle->SetCanvasDefW(540);

   myStyle->SetPadBottomMargin(0.1);
   myStyle->SetPadTopMargin(0.1);
   myStyle->SetPadLeftMargin(0.1);
   myStyle->SetPadRightMargin(0.1);


   myStyle->SetPadTickX(1);
   myStyle->SetPadTickY(1);

   myStyle->SetFrameBorderMode(0);

   // US letter
   myStyle->SetPaperSize(20, 24);


    gROOT->SetStyle("MyStyle"); //uncomment to set this style
}

string getCuts()
{
	string cut = "B0_mbc > 5.27 && B0_deltae > -0.2 && B0_deltae < 0.1"; // BEST
	cut += " && abs(B0_K_10_K_S0_M - 0.497) < 0.01 ";
	cut += " && B0_DeltaTErr < 2.5 && B0_DeltaTErr > 0";
	cut += " && B0_DeltaT < 10 && B0_DeltaTErr > -10";
	cut += " && B0_m12 > 0.6 && B0_m12 < 0.9";
	//cut += " && (B0_m23 > 0.95 ||  B0_m23 < 0.85) && (B0_m13 > 0.95 ||  B0_m13 < 0.85)";
	cut += " && B0_K_10_M < 1.8 ";
	cut += " && B0_K_10_K_S0_Rho > 0.15";
	//cut += " && abs(B0_FANN_qrCombined) > 0.5 ";
	return cut;
}

void makePretty(TH1F* htemp, int color = kBlue)
{
	htemp->SetLineWidth(3);
	htemp->SetFillStyle(3004);
	htemp->SetFillColor(color);
	htemp->SetLineColor(color);
	if(color != kBlue)
	{
		htemp->SetFillStyle(3005);
	}
}
