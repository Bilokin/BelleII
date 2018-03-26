from ROOT import gROOT, TFile, TTree 
def prepareFiles(fileinputname, fileoutputname):
    fileinput = TFile(fileinputname)
    tree = fileinput.Get("B0Signal")
    fileoutput = TFile(fileoutputname,"recreate")
    treeout = tree.CopyTree('B0_isSignal || B0_isContinuumEvent')
    treeout.Write()
    fileoutput.Write()
    fileoutput.Close()
    fileinput.Close()
prepareFiles('/home/ilc/bilokin/production-xsd/mva-addition/train.root', '/home/ilc/bilokin/production-xsd/mva-addition/trainStripped.root')
prepareFiles('/home/ilc/bilokin/production-xsd/mva-addition/test.root', '/home/ilc/bilokin/production-xsd/mva-addition/testStripped.root')
