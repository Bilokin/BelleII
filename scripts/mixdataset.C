void mixdataset(string fullfilename="official-mc101/all--merged-1iab-kspipigamma-mc10.root", string signalfilename = "try-CPMIX/merged-signal-v3-bkg-nocharge.root", float nsignal = 250)
{
	string outputfilename = "root/tmp-dataset-v3-signalonly.root";
	TFile * fileOut = new TFile(outputfilename.c_str(),"recreate");
	TFile * fileFull = TFile::Open(fullfilename.c_str(),"read");
	TFile * fileSignal = TFile::Open(signalfilename.c_str(),"read");
	string cutsignal = "!B0_isSignal";
	string cutbkg = "B0_isSignal";
	fileFull->cd();
	TTree * B0SignalFull = (TTree*)fileFull->Get("B0Signal");
	fileSignal->cd();
	TTree * B0SignalSignal = (TTree*)fileSignal->Get("B0Signal");
	TTree * B0SignalSignalStripped = B0SignalSignal->CopyTree(cutbkg.c_str());
	int nevents = B0SignalSignalStripped->GetEntries();
	cout << "Nevents: " << nevents << endl;
	TTree * newsignaltree = B0SignalSignalStripped->CloneTree(nsignal);
	TTree * B0SignalFullStripped = B0SignalFull->CopyTree(cutsignal.c_str());
	fileOut->cd();
	TList * list = new TList; 
	list->Add(B0SignalFullStripped); 
	list->Add(newsignaltree);
	TTree * newtree = TTree::MergeTrees(list); 
	newtree->AutoSave();
	newtree->Write();
	fileOut->Write();
	fileOut->Close();
	fileFull->Close();
	fileSignal->Close();
	cout << "Nevents: " << nevents << endl;

}
