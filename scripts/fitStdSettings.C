
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
	settings.fContinuum = 0.6712;//0.763;
	settings.dw = -0.013;//-0.005;
	settings.w = 0.2;
	settings.fbkg =     {3.9203e-01, 3.2794e-01};
	settings.sigmabkg = {1.5836,  6.2997e-01, 4.9696};
	settings.fres =     {0.400615, 0.407007, 0.192379};//
	//settings.fres =     {0.421832, 0.186005, 0.392162};
	settings.sigmares = { 0.632337, 1.60292, 4.9265};//
	//settings.sigmares = { 5, 5, 5};
	settings.wvalues ={ 0.473011, 0.363004, 0.314189, 0.194, 0.183236, 0.0518359, 0.0227273};//pr segmenter le w pdf

	settings.mbcBBbkgPar = {5.2793, 4.6633e-03, -6.6620e+01, 0.187172};	//sigmean sigsigma argpar fbkg
	settings.deBBbkgPar = {6.8044e-03, 1.0017e-01};//c0 c1
	settings.csBBbkgPar = {4.8948e-01, 4.6557e-01, 1.2732, 4.0000e+01};//mean sigma alpha n
	settings.dtBBbkgPar = {5.2371e-01, 2.7222e-01, 1.5836,  6.2997e-01, 4.9696, 1.4829};//mgbkg fbkg1 2 sgbkg 1 2 3 sgbkgpar
	return settings;
}
#endif
