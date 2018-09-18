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
from ROOT import gROOT, TFile, TTree                                                                                             
import sysconfig
gROOT.ProcessLine(".include " + sysconfig.get_path("include"))
from flavorTagger import *

from beamparameters import *
# check if the required input file exists (from B2A101 example)
import os.path
import sys
#from pi0etaveto import writePi0EtaVeto, myVetoVariables

ratingVar = "chiProb"
defaultInputFilename = "evtgen.root"
defaultInputFoldername = "test"
inputFilename = defaultInputFoldername + '/' + defaultInputFilename
defaultOutputFilename = "test.root"
defaultOutputFoldername = "."
outputFilename = defaultOutputFoldername + '/' + defaultOutputFilename

enableMCTruth = True
# Change K resonance name here:
Kres = 'Xsd'
for arg in sys.argv:
	print(arg)
if len(sys.argv)==2:
	inputFilename = sys.argv[1]
if len(sys.argv)==3:
	inputFilename = sys.argv[1]
	outputFilename = sys.argv[2]
#use_central_database("GT_gen_prod_003.11_release-00-09-01-FEI-a")
#use_central_database("GT_gen_prod_004.11_Master-20171213-230000")
#use_central_database("GT_gen_prod_004.10_release-01-00-00")
use_central_database("analysis_AAT-parameters_release-01-02-03")
from variables import variables
variables.addAlias('pi0Likeness','extraInfo(Pi0_Prob)')
variables.addAlias('etaLikeness','extraInfo(Eta_Prob)')
variables.addAlias('myRating','extraInfo(myRating)')
variables.addAlias('myRatingCriteria',ratingVar)
variables.addAlias('B0_CosTBTO','cosTBTO')
variables.addAlias('B0_cc2','CleoCone(2)')
variables.addAlias('B0_hso12','KSFWVariables(hso12)')
variables.addAlias('B0_R2','R2')
variables.addAlias('B0_hso10','KSFWVariables(hso10)')
variables.addAlias('B0_hso14','KSFWVariables(hso14)')
variables.addAlias('B0_cc9','CleoCone(9)')
variables.addAlias('B0_CosTBz','cosTBz')
variables.addAlias('B0_ThrustO','thrustOm')
variables.addAlias('B0_ThrustB','thrustBm')
variables.addAlias('B0_cc4','CleoCone(4)')
variables.addAlias('B0_hso02','KSFWVariables(hso02)')
variables.addAlias('CSMVA','extraInfo(CSMVA)')
variables.addAlias('MCFDmy','mcFlightDistanceOfDaughter(0,2)')
variables.addAlias('MCFTmy','mcFlightTimeOfDaughter(0,2)')
variables.addAlias('FDmy','flightDistanceOfDaughter(0,2)')
variables.addAlias('FTmy','flightTimeOfDaughter(0,2)')
variables.addAlias('FDEmy','flightDistanceOfDaughterErr(0,2)')
variables.addAlias('FTEmy','flightTimeOfDaughterErr(0,2)')

add_beamparameters(analysis_main,'Y4S')
inputMdst('default', inputFilename)
stdKshorts()
stdPhotons('loose')
stdPi('99eff')
#fillParticleList('pi+:99eff', 'pionID > 0.002 and chiProb > 0.001', True)
applyCuts('gamma:loose','1.4 < E < 4')
krescuts = "0.5 < M < 2.0 \
and daughter(2,significanceOfDistance) > 5 \
and daughter(2,dM) < 0.015 and daughter(2,dM) > -0.015 \
and daughter(0,pionID) > 0.2 and daughter(1,pionID) > 0.2 \
and daughterInvM(0,1) > 0.6 and daughterInvM(0,1) < 0.9 \
"
#and daughter(0,piid) > 0.2 and daughter(1,piid) > 0.2 \ MC9
vertexKFit('K_S0:all',0.0)
reconstructDecay(Kres+":all -> pi+:99eff pi-:99eff K_S0:all", krescuts)
reconstructDecay("B0:signal -> "+Kres+":all gamma:loose", "Mbc > 5.2 and deltaE < 0.2 and deltaE > -0.2 and  -0.65 < daughter(1, cosTheta) < 0.85")
vertexRave('B0:signal',0.0001, 'B0 -> ['+Kres+' -> ^pi+ ^pi- ^K_S0] gamma')
#vertexTree('B0:signal',0.0001)

rankByHighest('B0:signal',ratingVar, 1, outputVariable='myRating')

buildRestOfEvent('B0:signal')

oldMask = ('oldMask', 'useCMSFrame(p)<=3.2', 'p >= 0.05 and useCMSFrame(p)<=3.2')

# define the "cleaner" mask
eclCut = '[E > 0.062 and abs(clusterTiming) < 18 and clusterReg==1] or \
[E>0.060 and abs(clusterTiming) < 20 and clusterReg==2] or \
[E>0.056 and abs(clusterTiming) < 44 and clusterReg==3]'
cleanMask = ('cleanMask', 'abs(d0) < 10.0 and abs(z0) < 20.0', eclCut)

# append both masks to ROE
appendROEMasks('B0:signal', [oldMask,cleanMask])

# choose one mask which is applied
buildContinuumSuppression('B0:signal', 'cleanMask')
if enableMCTruth:
	matchMCTruth('B0:signal')

TagV('B0:signal', 'breco')

flavorTagger(particleLists = 'B0:signal', weightFiles='B2JpsiKs_muBGx1')
#matchMCTruth('pi+:all')
#matchMCTruth(Kres+':all')
#matchMCTruth('gamma:loose')
analysis_main.add_module('MVAExpert', listNames=['B0:signal'], extraInfoName='CSMVA',
		identifier='./mva-addition/MyTMVA.xml')

writePi0EtaVeto('B0:signal', 'B0 -> '+Kres+' ^gamma')
#myVetoVariables(Kres)

toolsB0_meson =  ['Kinematics','^B0 -> [^'+Kres+' -> ^pi+ ^pi- [ ^K_S0 ->  ^pi+ ^pi- ] ] ^gamma']
toolsB0_meson += ['CustomFloats[cosTheta:isSignal:isContinuumEvent:myRating:myRatingCriteria]', '^B0']
toolsB0_meson += ['CustomFloats[pi0Likeness:etaLikeness]', '^B0']
toolsB0_meson += ['CustomFloats[CSMVA]', '^B0']
toolsB0_meson += ['Vertex','^B0 -> ['+Kres+' -> pi+ pi- ^K_S0] gamma']
toolsB0_meson += ['CustomFloats[significanceOfDistance:dM]', 'B0 -> ['+Kres+' -> pi+ pi- ^K_S0] gamma']
toolsB0_meson += ['MCVertex','^B0 -> ['+Kres+' -> pi+ pi- ^K_S0] gamma']
toolsB0_meson += ['InvMass','^B0 -> [^'+Kres+' -> pi+ pi- ^K_S0]  gamma']
toolsB0_meson += ['DeltaEMbc','^B0']
toolsB0_meson += ['PID','B0 -> ['+Kres+' -> ^pi+ ^pi- [ K_S0 ->  ^pi+ ^pi- ] ] gamma']
toolsB0_meson += ['CustomFloats[chiProb]','B0 -> ['+Kres+' -> ^pi+ ^pi- [ K_S0 ->  ^pi+ ^pi- ] ] gamma']
toolsB0_meson += ['CustomFloats[cosTheta:isSignal]', 'B0 -> [^'+Kres+' -> ^pi+ ^pi- [ ^K_S0 ->  ^pi+ ^pi- ] ] ^gamma']
toolsB0_meson += ['CustomFloats[d0:z0:firstPXDLayer:firstSVDLayer]', 'B0 -> ['+Kres+' -> ^pi+ ^pi- [ K_S0 ->  ^pi+ ^pi- ] ] gamma']
toolsB0_meson += ['CustomFloats[minC2HDist:clusterSecondMoment:clusterErrorTiming:clusterTiming:clusterE1E9:clusterE9E21:clusterAbsZernikeMoment40:clusterAbsZernikeMoment51]', 'B0 -> ['+Kres+' -> pi+ pi- K_S0] ^gamma']
toolsB0_meson += ['CustomFloats[distance]', 'B0 -> ['+Kres+' -> pi+ pi- ^K_S0] gamma']
toolsB0_meson += ['Dalitz', '^B0 -> ['+Kres+' -> ^pi+ ^pi- ^K_S0] gamma']
toolsB0_meson += ['TrackHits','B0 -> ['+Kres+' -> ^pi+ ^pi-  [ K_S0 ->  ^pi+ ^pi- ] ] gamma']
toolsB0_meson += ['TagVertex', '^B0']
toolsB0_meson += ['MCTagVertex', '^B0']
toolsB0_meson += ['DeltaT', '^B0']
toolsB0_meson += ['MCDeltaT', '^B0']
toolsB0_meson += ['FlavorTagging', '^B0']
toolsB0_meson += ['ContinuumSuppression', '^B0']
toolsB0_meson =  ['CMSKinematics','B0 -> [^'+Kres+' -> pi+ pi- [ K_S0 ->  pi+ pi- ] ] ^gamma']
toolsB0_meson += ['EventMetaData', '^B0']

ntupleFile(outputFilename)
ntupleTree('B0Signal', 'B0:signal', toolsB0_meson)
# Process the events
process(analysis_main)

# print out the summary
print(statistics)
