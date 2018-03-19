/*! \struct fitSettings_t
 *  \brief Brief struct description
 *
 *  Detailed description
 *  Original version: Sviatoslav Bilokin, 2017/11/27
 *  Modifications:
 *   2017/11/30 JB
 */


#ifndef __tdcpvSettings_C_
#define __tdcpvSettings_C_
#include <iostream>

struct tdcpvSettings{
  float lumi = 1.0; // ab-1
  float Agen = 0.0;
  float Sgen = 0.0;
	float fsig = 0.0;
  float w = 0.0;
  float dw = 0.0;
	vector<float> fbkg;// {0.0, 0.0, 0.0, 0.0, 0.0};
	vector<float> sigmabkg;//[5] = {0.0, 0.0, 0.0, 0.0, 0.0};
	vector<float> fres;//[5]     = {0.0, 0.0, 0.0, 0.0, 0.0};
	vector<float> sigmares;//[5] = {0.0, 0.0, 0.0, 0.0, 0.0};
  float Ainit = 0.0;
  float Sinit = 0.0;
  float fsiginit = 0.0;
  float winit = 0.0;
  float dwinit = 0.0;
  int   ifDisplay = 1;
  int   ifVerbose = 1;
  
	void Print()
	{
    std::cout << "Equivalent luminosity = " << lumi << " ab-1" << endl;
    std::cout << "Generation with A = " << Agen;
    std::cout << " and S = " << Sgen << std::endl;
		std::cout << "Signal fraction: " << fsig << std::endl;
		std::cout << "Flavor contamination w: " << w << " dw: " << dw << std::endl;
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
    std::cout << "Fit starting with A = " << Ainit;
    std::cout << " and S = " << Sinit << std::endl;
    std::cout << "Signal fraction: " << fsiginit << std::endl;
    std::cout << "Flavor contamination w: " << winit << " dw: " << dwinit << std::endl;
		std::cout << std::endl;
	}
} /* optional variable list */;

#endif
