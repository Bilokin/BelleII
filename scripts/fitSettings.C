/*! \struct fitSettings_t
 *  \brief Brief struct description
 *
 *  Detailed description
 */


#ifndef __fitSettings_C_
#define __fitSettings_C_
struct fitSettings {
	float fsig = 0.0;
	vector<float> fbkg;// {0.0, 0.0, 0.0, 0.0, 0.0};
	vector<float> sigmabkg;//[5] = {0.0, 0.0, 0.0, 0.0, 0.0};
	vector<float> fres;//[5]     = {0.0, 0.0, 0.0, 0.0, 0.0};
	vector<float> sigmares;//[5] = {0.0, 0.0, 0.0, 0.0, 0.0};
	float w = 0.0;
	float dw = 0.0;
	vector<float> wparameters;
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
	}
} /* optional variable list */;

#endif
