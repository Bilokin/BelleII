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
use_central_database("GT_gen_prod_004.10_release-01-00-00")
from variables import variables
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

add_beamparameters(analysis_main,'Y4S')
inputMdst('default', inputFilename)
stdKshorts()
stdPhotons('loose')
stdPi0s()
stdPi('99eff')
applyCuts('gamma:loose','1.4 < E < 4')
krescuts = "0.5 < M < 2.0 \
and daughter(2,significanceOfDistance) > 3 \
and daughter(2,dM) < 0.011 and daughter(2,dM) > -0.011 \
and daughter(2,dr) > 0.05 \
and daughterInvM(0,1) > 0.6 and daughterInvM(0,1) < 0.9 \
"
#vertexKFit('K_S0:all',0.0)
reconstructDecay(Kres+":all -> pi+:99eff pi-:99eff K_S0:all", krescuts)
reconstructDecay("B0:signal -> "+Kres+":all gamma:loose", "Mbc > 5.2 and deltaE < 0.2 and deltaE > -0.2")
vertexRave('B0:signal',0.01, 'B0 -> ['+Kres+' -> ^pi+ ^pi- ^K_S0] gamma')
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

matchMCTruth('B0:signal')

TagV('B0:signal', 'breco')

#flavorTagger(particleLists = 'B0:signal', weightFiles='B2JpsiKs_muBGx1')
#matchMCTruth('pi+:all')
#matchMCTruth(Kres+':all')
#matchMCTruth('gamma:loose')
analysis_main.add_module('MVAExpert', listNames=['B0:signal'], extraInfoName='CSMVA',
		identifier='./mva-addition/MyTMVA.xml')

writePi0EtaVeto('B0:signal', 'B0 -> '+Kres+' ^gamma')
myVetoVariables(Kres)

toolsB0_meson =  ['Kinematics','^B0 -> [^'+Kres+' -> ^pi+ ^pi- [ ^K_S0 ->  ^pi+ ^pi- ] ] ^gamma']
toolsB0_meson += ['CustomFloats[cosTheta:isSignal:isContinuumEvent:myRating:myRatingCriteria]', '^B0']
toolsB0_meson += ['CustomFloats[pi0veto_M:pi0veto_gamma0_E:pi0veto_gamma1_E:pi0veto_mcPDG:pi0veto_cosTheta:pi0veto_gamma1_cosTheta:pi0veto_gamma1_clusterE1E9:pi0veto_gamma1_clusterE9E21:pi0veto_gamma1_clusterTiming:pi0veto_gamma1_clusterAZM40:pi0veto_gamma1_clusterAZM51:pi0veto_gamma1_clusterSecondMoment]', '^B0']
toolsB0_meson += ['CustomFloats[eta0veto_M:eta0veto_gamma0_E:eta0veto_gamma1_E:eta0veto_mcPDG:eta0veto_cosTheta:eta0veto_gamma1_cosTheta:eta0veto_gamma1_clusterE1E9:eta0veto_gamma1_clusterE9E21:eta0veto_gamma1_clusterTiming:eta0veto_gamma1_clusterAZM40:eta0veto_gamma1_clusterAZM51:eta0veto_gamma1_clusterSecondMoment]', '^B0']
toolsB0_meson += ['CustomFloats[pi0Likeness:etaLikeness]', '^B0']
toolsB0_meson += ['CustomFloats[CSMVA]', '^B0']
toolsB0_meson += ['CustomFloats[pi0vetoGamma1E:pi0vetoGamma1Timing:pi0vetoGamma1CosTheta]', '^B0']
#toolsB0_meson += ['MCKinematics','^B0 ->  ^'+Kres+' gamma']
toolsB0_meson += ['MCTruth','^B0 -> [^'+Kres+' -> ^pi+ ^pi- ^K_S0] ^gamma']
toolsB0_meson += ['MCHierarchy','B0 -> ['+Kres+' -> ^pi+ ^pi- ^K_S0] ^gamma']
toolsB0_meson += ['Vertex','^B0 -> ['+Kres+' -> pi+ pi- ^K_S0] gamma']
toolsB0_meson += ['CustomFloats[significanceOfDistance:dM]', '^B0 -> ['+Kres+' -> pi+ pi- ^K_S0] gamma']
toolsB0_meson += ['MCVertex','^B0 -> ['+Kres+' -> pi+ pi- ^K_S0] gamma']
toolsB0_meson += ['InvMass','^B0 -> [^'+Kres+' -> pi+ pi- ^K_S0]  gamma']
toolsB0_meson += ['DeltaEMbc','^B0']
toolsB0_meson += ['PID','B0 -> ['+Kres+' -> ^pi+ ^pi- [ K_S0 ->  ^pi+ ^pi- ] ] gamma']
toolsB0_meson += ['CustomFloats[chiProb]','B0 -> ['+Kres+' -> ^pi+ ^pi- [ K_S0 ->  ^pi+ ^pi- ] ] gamma']
toolsB0_meson += ['CustomFloats[cosTheta:isSignal]', 'B0 -> [^'+Kres+' -> ^pi+ ^pi- [ ^K_S0 ->  ^pi+ ^pi- ] ] ^gamma']
toolsB0_meson += ['CustomFloats[d0:z0]', 'B0 -> ['+Kres+' -> ^pi+ ^pi- [ K_S0 ->  ^pi+ ^pi- ] ] gamma']
toolsB0_meson += ['CustomFloats[minC2HDist:clusterMergedPi0:clusterSecondMoment:clusterErrorTiming:clusterTiming:clusterE1E9:clusterE9E21:clusterAbsZernikeMoment40:clusterAbsZernikeMoment51]', 'B0 -> ['+Kres+' -> pi+ pi- K_S0] ^gamma']
toolsB0_meson += ['CustomFloats[useCMSFrame(daughterAngleInBetween(0,1)):cosHelicityAngle]', 'B0 -> [^'+Kres+' -> pi+ pi- K_S0] gamma']
toolsB0_meson += ['Dalitz', '^B0 -> ['+Kres+' -> ^pi+ ^pi- ^K_S0] gamma']
toolsB0_meson += ['TrackHits','B0 -> ['+Kres+' -> ^pi+ ^pi-  [ K_S0 ->  ^pi+ ^pi- ] ] gamma']
toolsB0_meson += ['TagVertex', '^B0']
toolsB0_meson += ['MCTagVertex', '^B0']
toolsB0_meson += ['DeltaT', '^B0']
#toolsB0_meson += ['DeltaTErr', '^B0']
toolsB0_meson += ['MCDeltaT', '^B0']
toolsB0_meson += ['FlavorTagging', '^B0']
toolsB0_meson += ['ContinuumSuppression', '^B0:phiKs']
toolsB0_meson += ['MassBeforeFit', '^B0']
toolsB0_meson += ['ROEMultiplicities', '^B0']
toolsB0_meson += ['EventMetaData', '^B0']

gammaInfo =  ['Kinematics','^gamma']
gammaInfo += ['MCKinematics','^gamma']
gammaInfo += ['MCTruth','^gamma']
gammaInfo += ['MCHierarchy','^gamma']
gammaInfo += ['PDGCode','^gamma']
gammaInfo += ['InvMass','^gamma']
gammaInfo += ['DeltaEMbc','^gamma']
gammaInfo += ['CustomFloats[cosTheta:phi]', '^gamma']

PiInfo  = ['Kinematics','^pi+']
PiInfo += ['MCKinematics','^pi+']
PiInfo += ['MCKinematics','^pi+']
PiInfo += ['MCTruth','^pi+']
PiInfo += ['CustomFloats[genMotherPDG:d0:z0:d0Err:z0Err:cosTheta:phi]', '^pi+']
PiInfo += ['Reconstructible', '^pi+']
PiInfo += ['MCHierarchy','^pi+']
#PiInfo += ['MCVertex','^pi+']
PiInfo += ['Track','^pi+']
PiInfo += ['TrackHits','^pi+']
#PiInfo += ['PID','^pi+']

K0Info =  ['Kinematics','^K_S0 -> ^pi+ ^pi-']
K0Info += ['MCKinematics','^K_S0 -> ^pi+ ^pi-']
K0Info += ['MCTruth','^K_S0 -> ^pi+ ^pi-']
K0Info += ['InvMass','^K_S0']
K0Info += ['Vertex', '^K_S0']
K0Info += ['MCVertex', '^K_S0']
K0Info += ['CustomFloats[cosTheta]', '^K_S0 -> ^pi+ ^pi-']
K0Info += ['TrackHits','K_S0 -> ^pi+ ^pi-']
K0Info += ['CustomFloats[d0:z0:d0Err:z0Err]', 'K_S0 -> ^pi+ ^pi-']

K0starInfo =  ['Kinematics','^'+Kres+' ->  ^pi+ ^pi- ^K_S0']
K0starInfo += ['MCKinematics','^'+Kres]
K0starInfo += ['MCTruth','^'+Kres]
K0starInfo += ['InvMass','^'+Kres]
K0starInfo += ['Vertex','^'+Kres]

#pi0Info = ['Kinematics','^pi0 -> ^gamma ^gamma']#pi0Info += ['MCTruth','^pi0 -> ^gamma ^gamma']#pi0Info += ['InvMass','^pi0']
#pi0Info += ['CustomFloats[cosTheta:clusterAbsZernikeMoment40:clusterAbsZernikeMoment51:clusterE1E9:clusterE9E21:clusterSecondMoment:clusterMergedPi0:clusterTiming:clusterBelleQuality:clusterErrorTiming]','pi0 -> ^gamma ^gamma']
#eta0Info = ['Kinematics','^eta -> ^gamma ^gamma']#eta0Info += ['MCTruth','^eta -> ^gamma ^gamma']#eta0Info += ['InvMass','^eta']
#eta0Info += ['CustomFloats[cosTheta:clusterAbsZernikeMoment40:clusterAbsZernikeMoment51:clusterE1E9:clusterE9E21:clusterSecondMoment:clusterMergedPi0:clusterTiming:clusterBelleQuality:clusterErrorTiming]','eta -> ^gamma ^gamma']

ntupleFile(outputFilename)
ntupleTree('B0Signal', 'B0:signal', toolsB0_meson)
#ntupleTree('pions', 'pi+:all', PiInfo)
#ntupleTree('K0Signal', 'K_S0:all', K0Info)
#ntupleTree('Pi0Veto', 'pi0:veto', pi0Info)
#ntupleTree('Eta0Veto', 'eta:veto2', eta0Info)
#ntupleTree('GammaSignal', 'gamma:loose', gammaInfo)
#ntupleTree('K1Signal', ''+Kres+':all', K0starInfo)
# Process the events
process(analysis_main)

# print out the summary
print(statistics)
