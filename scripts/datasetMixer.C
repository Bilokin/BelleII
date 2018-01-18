void resizeFile(string inname = "test.root", string outname = "outtest.root", float fraction = 0.5)
{
	std::cout << "File: " << inname << " resize factor: " << fraction << std::endl;
	TFile * infile = TFile::Open(inname.c_str());
        TTree* B0Signal = (TTree*)infile->Get("B0Signal");
	int nevents = B0Signal->GetEntries();
	std::cout << "\tN entries: " << nevents << std::endl;
	TFile * outfile = new TFile(outname.c_str(),"recreate");
	TTree *newtree = B0Signal->CloneTree(nevents*fraction);
	std::cout << "\tN entries: " << newtree->GetEntries() << std::endl;
	newtree->Write();
	outfile->Write();
	outfile->Close();
	infile->Close();
}

void datasetMixer(string mixedname = "mixed/mixed.root",string chargedname = "charged/charged.root" , string lightname = "light/light.root", string ccbarname = "ccbar/ccbar.root")
{
	//int signalK1Nevents = 1e7;
	//float sigmaY4S = 0.5346e-9;
	//float BRK1gamma = 0.0003118;
	//int signalK1Nevents = 329;
	//float signalK1Xs = sigmaY4S*BRK1gamma*2;
	//std::cout << "K1 cross section:   \t" << signalK1Xs*1e18 << " ab" << std::endl;
	//float signalK1Lumi = (float)signalK1Nevents / signalK1Xs;
	
	float ccbarXs = 1.329e-9;
	float uubarXs = 1.605e-9;
	float ssbarXs = .38e-9;
	float ddbarXs = .4e-9;
	float mixedXs = .565e-9;
	float chargedXs = .535e-9;
	int ccbarNevents = 1329e6*0.8;
	int ssbarNevents = 170308794+119590617;
	int ddbarNevents = 0;
	int uubarNevents = 1.22749e+09;
	int mixedNevents = 415360000;
	int chargedNevents = 360000000;
	float factor = 1e-18;
	
	
	float chargedLumi = (float)chargedNevents /chargedXs * factor;
	float mixedLumi = (float)mixedNevents / mixedXs * factor;
	float ccbarLumi = (float)ccbarNevents / ccbarXs * factor;
	float lightLumi = (float)(ssbarNevents+ddbarNevents+uubarNevents) / (uubarXs +ddbarXs+ssbarXs)* factor;

	//std::cout << "K1 integrated lumi:  \t" << signalK1Lumi*1e-18 << " ab-1" << std::endl;
	std::cout << "ccbar integrated lumi:  \t" << ccbarLumi << " ab-1" << std::endl;
	std::cout << "light integrated lumi:  \t" << lightLumi << " ab-1" << std::endl;
	std::cout << "charged integrated lumi:\t" << chargedLumi << " ab-1" << std::endl;
	std::cout << "mixed integrated lumi:  \t" << mixedLumi << " ab-1" << std::endl;
	float targetLumi = 0.63;
	bool resize = 1;
	if (resize) 
	{
		resizeFile(mixedname, "mixed/lumi630fb.root", targetLumi/mixedLumi);
		resizeFile(chargedname, "charged/lumi630fb.root", targetLumi/chargedLumi);
		resizeFile(ccbarname, "ccbar/lumi630fb.root", targetLumi/ccbarLumi);
		resizeFile(lightname, "light/lumi630fb.root", targetLumi/lightLumi);
	}
	
}
