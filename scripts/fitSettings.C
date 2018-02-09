/*! \struct fitSettings_t
 *  \brief Brief struct description
 *
 *  Detailed description
 */


#ifndef __fitSettings_C_
#define __fitSettings_C_
struct fitSettings {
	//
	bool useDeltaResolution = false;
	float fsig = 0.0;
	vector<float> fbkg;// {0.0, 0.0, 0.0, 0.0, 0.0};
	vector<float> sigmabkg;//[5] = {0.0, 0.0, 0.0, 0.0, 0.0};
	vector<float> fres;//[5]     = {0.0, 0.0, 0.0, 0.0, 0.0};
	vector<float> sigmares;//[5] = {0.0, 0.0, 0.0, 0.0, 0.0};
	float w = 0.0;
	float qbins[8] = { 0, 0.1, .25, 0.5, .625, .75, .875, 1};
	vector<float> wvalues;
	float dw = 0.0;
	vector<float> wparameters;
	//Mbc De parameters
	vector<float> deSigPar;
	vector<float> deBkgPar;
	vector<float> mbcSigPar; // CB pars: mean, sigma, alpha, n
	vector<float> mbcBkgPar; // Argus pars: limit
	void Print()
	{
		std::cout << "Signal fraction: " << fsig << std::endl;
		std::cout << "Flavor contamination w: " << w << " dw: " << dw << std::endl;
		std::cout << "w parameters: ";
		for (unsigned int i = 0; i < wparameters.size(); i++) 
		{
			std::cout <<  wparameters[i] << " ";
		}
		std::cout << std::endl;
		std::cout << "Signal resolution fractions:  ";
		for (unsigned int i = 0; i < fres.size(); i++) 
		{
			std::cout <<  fres[i] << " ";
		}
		std::cout << std::endl;
		std::cout << "Signal resolution parameters: "; 
		for (unsigned int i = 0; i < sigmares.size(); i++) 
		{
			std::cout <<  sigmares[i] << " ";
		}
		std::cout << std::endl;
		std::cout << "Background fractions:        ";
		for (unsigned int i = 0; i < fbkg.size(); i++) 
		{
			std::cout <<  fbkg[i] << " ";
		}
		std::cout << std::endl;
		std::cout << "Background parameters:       "; 
		for (unsigned int i = 0; i < sigmabkg.size(); i++) 
		{
			std::cout <<  sigmabkg[i] << " ";
		}
		std::cout << std::endl;
		std::cout << "w values:       "; 
		for (unsigned int i = 0; i < wvalues.size(); i++) 
		{
			std::cout <<  wvalues[i] << " ";
		}
		std::cout << std::endl;
	}
} /* optional variable list */;
fitSettings getStdSettings()
{
	fitSettings settings;
	//settings.mbcSigPar = {5.27952, 3.12605e-03, 1.2978, 1.837e1};
	settings.mbcSigPar = {5.27952, 3.02605e-03, 1.3, 20.};
	settings.mbcBkgPar = {5.29, -27};
	//settings.deSigPar = { -0.01, 4.16911e-02, 6.16997e-01, 1.0};
	settings.deSigPar = { -0.01, 4.111e-02, 5.8e-01, 20.0};
	settings.deBkgPar = {-2.1744e-01, 1.1122e-01, -6.1220e-03};
	settings.fsig = 0.5;
	settings.dw = -0.005;
	settings.w = 0.23;
	settings.fbkg =     {0.49, 0.51};
	settings.sigmabkg = {4.23,  1.4};
	settings.fres =     {0.421832, 0.186005, 0.392162};
	settings.sigmares = { 0.646568, 5, 1.64696};
	settings.wvalues ={ 0.475342, 0.38189, 0.339074, 0.192308, 0.166667, 0.0722101, 0.0193861 };
	return settings;
}
#endif
