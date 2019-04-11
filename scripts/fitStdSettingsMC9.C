
//
//      	  
//      II 
//                                
//      II  PPPP   HH  HH    CCCC   
//      II  PP  P  HH  HH  CC 
//      II  PPPP   HHHHHH  CC
//      II  PP     HH  HH  CC
//      II  PP     HH  HH    CCCC    STRASBOURG 2017
//      
//        
//                        Author: Bilokin S.    
//


#ifndef __fitStdSettings_C_
#define __fitStdSettings_C_
fitSettings getStdSettings()
{
	fitSettings settings;
	settings.mbcrange = {5.2, 5.3};
	settings.derange = {-0.2, 0.2};
	settings.csrange = {0, 2.5};

	settings.Nevents = 4482; //0.8 iab
	settings.Nexperiments = 0;
	//settings.mbcSigPar = {5.27952, 3.12605e-03, 1.2978, 1.837e1};
	settings.mbcSigPar = {5.27952, 3.02605e-03, 1.3, 20.};
	settings.mbcBkgPar = {5.29, -25};
	//settings.deSigPar = { -0.01, 4.16911e-02, 6.16997e-01, 1.0};
	settings.deSigPar = { -0.01, 4.111e-02, 5.8e-01, 20.0};
	settings.deBkgPar = {-0.16635, 0.055847, -4.8640e-02};
	//settings.deBkgPar = {0.1, 0.6,0.1};
	settings.csSigPar = {4.8763e-01, 9.6932e-01, 4.0885e-01, 3.2282e-01, 4.9590e-01};
	settings.csBkgPar = {-0.5, 7.5e-01}; //-2.2
	settings.fsig = 0.044;//mbc plot
	settings.fContinuum = 0.7;//0.763;
	settings.fBB = 0.1;//0.763;
	settings.fXsGamma = 0.087;//0.763;
	settings.fBB = 0.7;//0.763;
	settings.dw = -0.013;//-0.005;
	settings.w = 0.2;
	settings.fbkg =     {3.0095e-01, 6.2265e-01};
	settings.sigmabkg = {1.5836,  6.2997e-01, 4.9696};
	settings.fres =     {0.470973, 0.112987, 0.41604};//
	settings.sigmares = {0.47644, 2.80918, 1.08583};//
	//settings.fres =     {0.421832, 0.186005, 0.392162};
	//settings.sigmares = { 5, 5, 5};
	settings.wvalues ={ 0.473011, 0.363004, 0.314189, 0.194, 0.183236, 0.0518359, 0.0227273};//pr segmenter le w pdf

	settings.fbkgBB =   {6.3013e-01, 2.3109e-01};
	settings.mbcBBbkgPar = {5.2787e+00, 4.8427e-03, -7.8275e+01, 1.4857e-01};	//sigmean sigsigma argpar fbkg
	settings.deBBbkgPar = {-6.8829e-02, 2.1193e-01};//c0 c1
	settings.csBBbkgPar = {3.3802e-01, 6.1724e-01, 4.2479e+00, 3.7776e+01};//mean sigma alpha n
	settings.dtBBbkgPar = {5.2371e-01, 2.7222e-01, 1.5836,  6.2997e-01, 4.9696, 1.4829};//mgbkg fbkg1 2 sgbkg 1 2 3 sgbkgpar
	settings.fbkgXsGamma =   {6.1032e-01, 2.8473e-01};
	settings.mbcXsGammabkgPar = {5.2822e+00, 3.1518e-03, -6.7289e+01, 6.0744e-02};	//sigmean sigsigma argpar fbkg
	settings.deXsGammabkgPar = {-5.1833e-01,-9.0639e-03};//c0 c1
	settings.csXsGammabkgPar = {3.1593e-01, 5.7550e-01, 1.4719e+00, 4.0000e+01};//mean sigma alpha n
	return settings;
}
#endif
