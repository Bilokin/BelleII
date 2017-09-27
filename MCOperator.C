void MCOperator(string filename = "B2A101-Y4SEventGeneration-evtgen.root")
{
	TFile * file = TFile::Open(filename.c_str());
	file->ls();
	int nparticles = 0;
	//TChain* T = new TChain("tree/MCParticles");
	//T->Add(filename.c_str());
	int mTotalNumberOfEvents = tree->GetEntries();
	tree->ls();	
	cout << "Size: " << mTotalNumberOfEvents << endl;	
//	tree->SetBranchAddress("MCParticles_", &nparticles);
	tree->Draw("MCParticles@.GetEntries()","","",1);
	//for(int i = 0; i < mTotalNumberOfEvents; i++)
	{
		tree->GetEntry(1);
		cout << nparticles << endl;
	}

	
}
