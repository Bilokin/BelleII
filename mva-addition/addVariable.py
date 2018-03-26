
from basf2 import *
# Avoid missing <pyconfig.h> error
from ROOT import gROOT, TFile, TTree, TCanvas                                                                                             
import sysconfig                                                                                                                   
gROOT.ProcessLine(".include " + sysconfig.get_path("include"))    
from array import array

import basf2_mva
import basf2_mva_util
import os.path
import sys
for arg in sys.argv:
	print(arg)
if len(sys.argv)==2:
	inputFilename = sys.argv[1]
def addCSMVA(inputfilename, xml='/home/sbilokin/BelleII/test/mva-addition/MyTMVAnew.xml'):
    basf2_mva.expert(basf2_mva.vector(xml),
        basf2_mva.vector(inputfilename), 'B0Signal', '/home/sbilokin/BelleII/test/mva-addition/expert.root')
    #os.system('cp -f ' + inputfilename + ' ' + outputfilename)
    f1 = TFile("/home/sbilokin/BelleII/test/mva-addition/expert.root","READ")
    f2 = TFile(inputfilename,"UPDATE")

    t1 = f1.Get("variables")
    t2 = f2.Get('B0Signal')

    branch = t1.GetListOfBranches().At(0).GetName() # get mva output variable
    mva=array('f',[0])
    t1.SetBranchAddress(branch, mva)

    newvar=array('f',[0])
    #newbranch = t2.GetBranch('B0_CSMVA').GetName()
    #if (newbranchname):
    #    print("Updating an existing branch.")
    #    t2.SetBranchAddress(newbranch.GetName(), newvar)
    #else:
    #    print("Creating a new branch.")
    newbranch = t2.Branch('B0_CSMVA2', newvar, 'B0_CSMVA2/F') # create new branch, same as from the expert
    # copy and fill for each entry
    for i in range( t1.GetEntries() ):
        t1.GetEntry(i)
        newvar[0] = mva[0]
        newbranch.Fill()
    #t2.AutoSave();
    t2.Write()
    f2.Write()
    f1.Close()
    f2.Close()
addCSMVA(inputFilename)
