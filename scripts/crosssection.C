void crosssection()
{
	//Cross section Y(4S) -> B0 B0
	float sigmaY4S = 0.5346e-9; // barn
	//Cross section Y(4S) -> B B
	//	 sigmaY4S = 1.1e-9; // barn
	float BRK1gamma = 0.0000135; // as in DECAY BELLE II DEC
	float BRK2gamma = 0.0000166; // as in DECAY BELLE II DEC
	//float BRKpipigamma = 1e-5; // as in PDG 
	float BRKpipigamma = 20.5e-6 /2 * 0.7; // as in G. Eigen 1510.04446 

	float phase3L = 0.8e18; // barn^-1
	phase3L = 0.55e18; // barn^-1 as in BABAR
	float fullL = 50e18; // barn^-1
	//Reconstruction efficiency times BR Kpipi gamma 
	float effK1gamma = 0.01;  
	float effK2gamma = 0.001;  
	float effKpipigamma = 0.04;  
	
	float Nevents = sigmaY4S*phase3L;
	float NK1gamma = 2*sigmaY4S*phase3L*BRK1gamma;
	float NK2gamma = 2*sigmaY4S*phase3L*BRK2gamma;
	float NKpipigamma = 2*sigmaY4S*phase3L*BRKpipigamma;
	float NKpipigammaSelect = 2*sigmaY4S*phase3L*BRKpipigamma*effKpipigamma;
	//float NKpipigamma2 = sigmaY4S*phase3L*(BRK1gamma*effK1gamma + BRK2gamma*effK2gamma);
	std::cout << "-------------------PHASE-III-------------------"  << std::endl;
	std::cout << "Total events:  	        " << Nevents << std::endl;
	std::cout << "Total K1gamma events:     " << NK1gamma << std::endl;
	std::cout << "Total K2gamma events:     " << NK2gamma << std::endl;
	std::cout << "Total Kpipigamma events:  " << NKpipigamma << std::endl;
	std::cout << "Used  Kpipigamma events:  " << NKpipigammaSelect << std::endl;
	//std::cout << "Total Kpipigamma' events: " << NKpipigamma2 << std::endl;
	std::cout << "-----------------------------------------------" << std::endl;
	Nevents = sigmaY4S*fullL;
	NK1gamma = 2*sigmaY4S*fullL*BRK1gamma;
	NK2gamma = 2*sigmaY4S*fullL*BRK2gamma;
	NKpipigamma = 2*sigmaY4S*fullL*BRKpipigamma;
	NKpipigammaSelect = 2*sigmaY4S*fullL*BRKpipigamma*effKpipigamma;
	//NKpipigamma2 = sigmaY4S*fullL*(BRK1gamma*effK1gamma + BRK2gamma*effK2gamma);
	std::cout << "----------------------FULL---------------------"  << std::endl;
	std::cout << "Total events:  	        " << Nevents << std::endl;
	std::cout << "Total K1gamma events:     " << NK1gamma << std::endl;
	std::cout << "Total K2gamma events:     " << NK2gamma << std::endl;
	std::cout << "Total Kpipigamma events:  " << NKpipigamma << std::endl;
	std::cout << "Used  Kpipigamma events:  " << NKpipigammaSelect << std::endl;
	//std::cout << "Total Kpipigamma' events: " << NKpipigamma2 << std::endl;
	std::cout << "-----------------------------------------------" << std::endl;
}
