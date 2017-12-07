void datasetMixer(string signalname = "test.root", string continuumname = "")
{
	int signalK1Nevents = 1e7;
	int ccbarNevents = 1039855979;

	float sigmaY4S = 0.5346e-9;
	float BRK1gamma = 0.0000135;
	
	float signalK1Xs = sigmaY4S*BRK1gamma*2;
	float ccbarXs = 1.329e-9;
	std::cout << "K1 cross section:   \t" << signalK1Xs*1e18 << " ab" << std::endl;
	float ccbarLumi = (float)ccbarNevents / ccbarXs;
	float signalK1Lumi = (float)signalK1Nevents / signalK1Xs;

	std::cout << "K1 integrated lumi:  \t" << signalK1Lumi*1e-18 << " ab-1" << std::endl;
	std::cout << "ccbar integrated lumi:\t" << ccbarLumi*1e-18 << " ab-1" << std::endl;
	
}
