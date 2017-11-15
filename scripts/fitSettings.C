/*! \struct fitSettings_t
 *  \brief Brief struct description
 *
 *  Detailed description
 */
struct fitSettings {
	float fsig = 0.0;
	float fbkg[5]     = {0.0, 0.0, 0.0, 0.0, 0.0};
	float sigmabkg[5] = {0.0, 0.0, 0.0, 0.0, 0.0};
	float fres[5]     = {0.0, 0.0, 0.0, 0.0, 0.0};
	float sigmares[5] = {0.0, 0.0, 0.0, 0.0, 0.0};
	float w = 0.0;
	float dw = 0.0;
	void Print()
	{
		std::cout << "Signal fraction: " << fsig << std::endl;
		std::cout << "Flavor contamination w: " << w << " dw: " << dw << std::endl;
		std::cout << "Signal resolution fractions:  ";
		for (unsigned int i = 0; i < 5; i++) 
		{
			std::cout <<  fres[i] << " ";
		}
		std::cout << std::endl;
		std::cout << "Signal resolution parameters: "; 
		for (unsigned int i = 0; i < 5; i++) 
		{
			std::cout <<  sigmares[i] << " ";
		}
		std::cout << std::endl;
		std::cout << "Background fractions:        ";
		for (unsigned int i = 0; i < 5; i++) 
		{
			std::cout <<  fbkg[i] << " ";
		}
		std::cout << std::endl;
		std::cout << "Background parameters:       "; 
		for (unsigned int i = 0; i < 5; i++) 
		{
			std::cout <<  sigmabkg[i] << " ";
		}
		std::cout << std::endl;
	}
} /* optional variable list */;
