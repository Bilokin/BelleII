#
#      	  
#      II 
#                                
#      II  PPPP   HH  HH    CCCC   
#      II  PP  P  HH  HH  CC 
#      II  PPPP   HH  HH  CC
#      II  PP     HHHHHH  CC
#      II  PP     HH  HH    CCCC    STRASBOURG 2017
#      
#        
#                        Author: Bilokin S.    
#
from basf2 import *
from modularAnalysis import *
from vertex import *
from stdV0s import stdKshorts
from stdPhotons import *
from stdCharged import *
from stdFSParticles import *
from ROOT import gROOT, TFile, TTree                                                                                             
import sysconfig
gROOT.ProcessLine(".include " + sysconfig.get_path("include"))
from flavorTagger import *

from beamparameters import *
# check if the required input file exists (from B2A101 example)
import os.path
import sys
from pi0etaveto import writePi0EtaVeto, myVetoVariables
ratingVar = "chiProb"
defaultInputFilename = "evtgen.root"
defaultInputFoldername = "test"
inputFilename = defaultInputFoldername + '/' + defaultInputFilename
defaultOutputFilename = "test.root"
defaultOutputFoldername = "."
outputFilename = defaultOutputFoldername + '/' + defaultOutputFilename
# Change K resonance name here:
Kres = 'K_10'
for arg in sys.argv:
	print(arg)
if len(sys.argv)==2:
	inputFilename = sys.argv[1]
if len(sys.argv)==3:
	inputFilename = sys.argv[1]
	outputFilename = sys.argv[2]
use_central_database("GT_gen_prod_003.11_release-00-09-01-FEI-a")
#use_central_database("GT_gen_prod_004.11_Master-20171213-230000")
from variables import variables

add_beamparameters(analysis_main,'Y4S')
inputMdst('default', inputFilename)
stdKshorts()
stdPhotons('loose')
stdPi0s()
stdPi('99eff')
applyCuts('gamma:loose','1.4 < E < 4')
krescuts = "0.5 < M < 2.5 \
and daughter(2,significanceOfDistance) > 3 \
and daughter(2,dM) < 0.011 and daughter(2,dM) > -0.011 \
and daughter(2,dr) > 0.05 \
and daughterInvM(0,1) > 0.6 and daughterInvM(0,1) < 0.9 \
"
reconstructDecay(Kres+":all -> pi+:99eff pi-:99eff K_S0:all", krescuts)
reconstructDecay("B0:signal -> "+Kres+":all gamma:loose", " 4 < M < 6 and Mbc > 5.2 and deltaE < 0.2 and deltaE > -0.2")
vertexRave('B0:signal',0.001, 'B0 -> ['+Kres+' -> ^pi+ ^pi- ^K_S0] gamma')
#vertexTree('B0:signal',0.0001)

rankByHighest('B0:signal',ratingVar, 100)
skimOutputUdst(outputFilename,['B0:signal'],[Kres+':all','K_S0:all','pi+:99eff'])
process(analysis_main)

# print out the summary
print(statistics)
