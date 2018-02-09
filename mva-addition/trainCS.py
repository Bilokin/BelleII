from basf2 import *
# Avoid missing <pyconfig.h> error
from ROOT import gROOT, TFile, TTree                                                                                             
import sysconfig                                                                                                                   
gROOT.ProcessLine(".include " + sysconfig.get_path("include"))    

import basf2_mva
import basf2_mva_util
trainVars = [
    'B0_ThrustB','B0_hso12','B0_CosTBTO','B0_cc2','B0_hso10','B0_cc9','B0_R2',
     'B0_hso14','B0_CosTBz', 'B0_cc4','B0_ThrustO','B0_hso02'#,'B0_hso14', 'B0_cosTheta', 'B0_mbc', 'B0_cc1'
]

trainData = ['/home/sbilokin/BelleII/test/mva-addition/trainStripped.root']
testData = ['/home/sbilokin/BelleII/test/mva-addition/testStripped.root']

# Global/General options
go = basf2_mva.GeneralOptions()
go.m_datafiles = basf2_mva.vector(*trainData)  # training sample
go.m_treename = "B0Signal" # ntuple tree name
go.m_identifier = "/home/sbilokin/BelleII/test/mva-addition/MyContinuum.xml" # name of the file with the trianing info
#go.m_identifier = "TMVA"

#go.m_identifier = "/home/ilc/bilokin/production-xsd/mva-addition/MyTMVA.xml" # name of the file with the trianing info

#go.m_identifier = "MVADatabaseIdentifier"
go.m_variables = basf2_mva.vector(*trainVars) # input variables
go.m_target_variable = "B0_isSignal" # target for training
go.m_weight_variable = ""

# Specific options
sp = basf2_mva.FastBDTOptions() # here we use the FastBDT method
#sp.m_nTrees = 100 # number of trees in the FastBDT forest
#sp.m_nCuts = 8 # number of cuts for each tree
#spbdt.m_shrinkage = 0.2 # i dont know...
#sp.m_nLevels = 4 # number of levels of a specific tree

#sp.m_nTrees = 100 # number of trees in the FastBDT forest
#sp.m_nCuts =  # number of cuts for each tree
#spbdt.m_shrinkage = 0.2 # i dont know...
#sp.m_nLevels = 4 # number of levels of a specific tree
tmva_bdt_options = basf2_mva.TMVAOptionsClassification()

basf2_mva.teacher(go,tmva_bdt_options)
