#include "Riostream.h"
#include "TString.h"
#include "TFile.h"
#include "TTree.h"
#include "TSystem.h"


TGraph * DrawGraph(string xname, string yname, TTree* tree)
{
    float time = 0.0;//
    float NimplantOuterVoltage = 0.0;//
    float NimplantInnerVoltage = 0.0;//
    float NimplantQuasiFermiPotential = 0.0;//
    float NimplantDisplacementCurrent = 0.0;//
    float NimplanteCurrent = 0.0;//
    float NimplanthCurrent = 0.0;//
    float NimplantTotalCurrent = 0.0;//
    float NimplantCharge  = 0.0;//
    float BSOuterVoltage  = 0.0;//
    float BSInnerVoltage = 0.0;//
    float BSQuasiFermiPotential  = 0.0;//
    float BSDisplacementCurrent = 0.0;//
    float BSeCurrent = 0.0;//
    float BShCurrent = 0.0;//
    float BSTotalCurrent = 0.0;//
    float BSCharge = 0.0;//

    tree->SetBranchAddress("time",&time);
    tree->SetBranchAddress("NimplantOuterVoltage",&NimplantOuterVoltage);
    tree->SetBranchAddress("NimplantInnerVoltage",&NimplantInnerVoltage);
    tree->SetBranchAddress("NimplantQuasiFermiPotential",&NimplantQuasiFermiPotential);
    tree->SetBranchAddress("NimplantDisplacementCurrent",&NimplantDisplacementCurrent);
    tree->SetBranchAddress("NimplanteCurrent",&NimplanteCurrent);
    tree->SetBranchAddress("NimplanthCurrent",&NimplanthCurrent);
    tree->SetBranchAddress("NimplantTotalCurrent",&NimplantTotalCurrent);
    tree->SetBranchAddress("NimplantCharge",&NimplantCharge);
    tree->SetBranchAddress("BSOuterVoltage",&BSOuterVoltage);
    tree->SetBranchAddress("BSInnerVoltage",&BSInnerVoltage);
    tree->SetBranchAddress("BSQuasiFermiPotential",&BSQuasiFermiPotential);
    tree->SetBranchAddress("BSDisplacementCurrent",&BSDisplacementCurrent);
    tree->SetBranchAddress("BSeCurrent",&BSeCurrent);
    tree->SetBranchAddress("BShCurrent",&BShCurrent);
    tree->SetBranchAddress("BSTotalCurrent",&BSTotalCurrent);
    tree->SetBranchAddress("BSCharge",&BSCharge);

    // push data to tree
    int entries = tree->GetEntries();
    vector <float> xvector;
    vector <float> yvector;
    for (Int_t i = -1; i <entries ; i++)
    {
        tree->GetEntry(i);
        xvector.push_back(NimplantTotalCurrent);
        yvector.push_back(BSeCurrent);
    }
    Float_t *x= &xvector[0];
    Float_t *y= &yvector[0];

    // plot graph
    //c1->cd();
    TGraph *g = new TGraph(entries, x, y);
    return g;
}

void TcadChoseBranch(const char *dirname="output/", const char *ext="_des.csv") {
    new TBrowser();// dont start without it

    auto c1 = new TCanvas("canvas_01", "spektrum", 550, 10, 800, 550);
    // add legend
    TLegend *legend = new TLegend(0.5,0.79,0.89,0.89);
    legend->SetBorderSize(0);

    // Read all file from dir
    TSystemDirectory dir(dirname, dirname);
    TList *files = dir.GetListOfFiles();
    if (files) {
        TSystemFile *file;
        TString fname;
        TIter next(files);
        string xname = "NimplantTotalCurrent";
        string yname = "BSeCurrent";
        int j = 0;
        while ((file = (TSystemFile *) next())) {
            fname = file->GetName();
            size_t lastindex = string(fname.Data()).rfind("$");
            TString rawname = string(fname.Data()).substr(0, lastindex);
	cout << "Rawname:  " << rawname << endl;
            if (!file->IsDirectory() && fname.EndsWith(ext)) {
                TFile *f = new TFile(rawname("[^.]+")+TString(".root"), "CREATE");//  new root file
                TTree *tree = new TTree(rawname, "data from csv file");
                tree->ReadFile((dirname + fname).Data(), "", ','); // read file, branch from firs line of file
		cout << "Input:  " << (dirname + fname).Data() << endl;
                tree->Print();
                if (j == 0) {


                    {
                        cout << xname << " - x value\n";

                        cout << yname << " - y value\n";

                    }

                    TGraph *g = DrawGraph(xname, yname, tree);
                    g->SetMarkerStyle(20);
                    g->SetMarkerColor(j + 100);
                    g->SetLineColor(j + 20);


                    if (j == 0) {
                        g->Draw("ALP");
                        legend->AddEntry(g,rawname,"l");
                        cout << "HERE" << endl; // test
                    }
                    else {
                        g->Draw("LPsame");
                        legend->AddEntry(g,rawname,"l");
                        cout << "HEREsame" << endl;// test
                    }

                    legend->Draw("same");
                    //c2->Print(fname.Data() + TString(".pdf"));
                    j++;
                }
            }
        }

        c1->Print(dirname+TString("Spectrum.pdf"));
        //f->Write();
    }
}
