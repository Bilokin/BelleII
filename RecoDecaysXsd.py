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
ratingVar = "chiProb"
defaultInputFilename = "evtgen.root"
defaultInputFoldername = "test"
inputFilename = defaultInputFoldername + '/' + defaultInputFilename
defaultOutputFilename = "test.root"
defaultOutputFoldername = "."
outputFilename = defaultOutputFoldername + '/' + defaultOutputFilename
for arg in sys.argv:
	print(arg)
if len(sys.argv)==2:
	inputFilename = sys.argv[1]
if len(sys.argv)==3:
	inputFilename = sys.argv[1]
	outputFilename = sys.argv[2]
#use_central_database("GT_gen_prod_003.11_release-00-09-01-FEI-a")
use_central_database("GT_gen_prod_004.11_Master-20171213-230000")
from variables import variables
variables.addAlias('myRating','extraInfo(myRating)')
variables.addAlias('pi0veto_M','extraInfo(pi0veto_M)')
variables.addAlias('pi0veto_mcPDG','extraInfo(pi0veto_mcPDG)')
variables.addAlias('pi0veto_cosTheta','extraInfo(pi0veto_cosTheta)')
variables.addAlias('pi0veto_gamma0_E','extraInfo(pi0veto_gamma0_E)')
variables.addAlias('pi0veto_gamma1_E','extraInfo(pi0veto_gamma1_E)')
variables.addAlias('pi0veto_gamma1_cosTheta','extraInfo(pi0veto_gamma1_cosTheta)')
variables.addAlias('pi0veto_gamma1_clusterE1E9','extraInfo(pi0veto_gamma1_clusterE1E9)')
variables.addAlias('pi0veto_gamma1_clusterE9E21','extraInfo(pi0veto_gamma1_clusterE9E21)')
variables.addAlias('pi0veto_gamma1_clusterTiming','extraInfo(pi0veto_gamma1_clusterTiming)')
variables.addAlias('pi0veto_gamma1_clusterAZM40','extraInfo(pi0veto_gamma1_clusterAbsZernikeMoment40)')
variables.addAlias('pi0veto_gamma1_clusterAZM51','extraInfo(pi0veto_gamma1_clusterAbsZernikeMoment51)')
variables.addAlias('pi0veto_gamma1_clusterSecondMoment','extraInfo(pi0veto_gamma1_clusterSecondMoment)')
variables.addAlias('eta0veto_gamma1_cosTheta','extraInfo(eta0veto_gamma1_cosTheta)')
variables.addAlias('eta0veto_gamma1_clusterE1E9','extraInfo(eta0veto_gamma1_clusterE1E9)')
variables.addAlias('eta0veto_gamma1_clusterE9E21','extraInfo(eta0veto_gamma1_clusterE9E21)')
variables.addAlias('eta0veto_gamma1_clusterTiming','extraInfo(eta0veto_gamma1_clusterTiming)')
variables.addAlias('eta0veto_gamma1_clusterAZM40','extraInfo(eta0veto_gamma1_clusterAbsZernikeMoment40)')
variables.addAlias('eta0veto_gamma1_clusterAZM51','extraInfo(eta0veto_gamma1_clusterAbsZernikeMoment51)')
variables.addAlias('eta0veto_gamma1_clusterSecondMoment','extraInfo(eta0veto_gamma1_clusterSecondMoment)')
variables.addAlias('eta0veto_M','extraInfo(eta0veto_M)')
variables.addAlias('eta0veto_mcPDG','extraInfo(eta0veto_mcPDG)')
variables.addAlias('eta0veto_cosTheta','extraInfo(eta0veto_cosTheta)')
variables.addAlias('eta0veto_gamma0_E','extraInfo(eta0veto_gamma0_E)')
variables.addAlias('eta0veto_gamma1_E','extraInfo(eta0veto_gamma1_E)')
variables.addAlias('myRatingCriteria',ratingVar)

add_beamparameters(analysis_main,'Y4S')
inputMdst('default', inputFilename)
stdKshorts()
stdPhotons('loose')
stdPi0s()
stdPi('all')
#applyCuts('gamma:loose','1.4 < E < 4')
#vertexRave('K_S0:all',0.01)
vertexKFit('K_S0:all',0.01)
reconstructDecay("Xsd:all -> pi+:all pi-:all K_S0:all", "0.5 < M < 2")
reconstructDecay("B0:signal -> Xsd:all gamma:loose", " 4 < M < 6 and Mbc > 5.2 and deltaE < 0.2 and deltaE > -0.2")
vertexRave('B0:signal',0.01, 'B0 -> [Xsd -> ^pi+ ^pi- ^K_S0] gamma')
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

flavorTagger(particleLists = 'B0:signal', weightFiles='B2JpsiKs_muBGx1')
matchMCTruth('pi+:all')
matchMCTruth('Xsd:all')
matchMCTruth('gamma:loose')


'''
fillSignalSideParticleList('gamma:sig', 'B0 -> Xsd ^gamma', path=analysis_main)
reconstructDecay('pi0:veto -> gamma:loose gamma:loose', '0.080 < M < 0.200')reconstructDecay('eta:veto2 -> gamma:loose gamma:loose', '0.45 < M < 0.65')
applyCuts('pi0:veto','daughter(0,E) > 1.4')applyCuts('eta:veto2','daughter(0,E) > 1.4')
rankByLowest('pi0:veto', 'abs(dM)', 1)rankByLowest('eta:veto2', 'abs(dM)', 1)
matchMCTruth('pi0:veto') matchMCTruth('eta:veto2') 
'''

#'''
# ----------------
# VETO starts here

roe_path = create_path()
deadEndPath = create_path()

signalSideParticleFilter('B0:signal', '', roe_path, deadEndPath)

fillParticleList('gamma:roe', 'isInRestOfEvent == 1 and E > 0.050 and cosTheta > -0.65', path=roe_path)

fillSignalSideParticleList('gamma:sig', 'B0 -> Xsd ^gamma', roe_path)

reconstructDecay('pi0:veto -> gamma:sig gamma:roe', '0.080 < M < 0.200', path=roe_path)
reconstructDecay('eta:veto2 -> gamma:sig gamma:roe', '0.45 < M < 0.65', path=roe_path)

rankByLowest('pi0:veto', 'abs(dM)', 1, path=roe_path)
rankByLowest('eta:veto2', 'abs(dM)', 1, path=roe_path)
matchMCTruth('pi0:veto', path=roe_path) 
matchMCTruth('eta:veto2', path=roe_path) 

variableToSignalSideExtraInfo('pi0:veto', {'M': 'pi0veto_M'}, path=roe_path)
variableToSignalSideExtraInfo('pi0:veto', {'daughter(0,E)': 'pi0veto_gamma0_E'}, path=roe_path)
variableToSignalSideExtraInfo('pi0:veto', {'daughter(1,E)': 'pi0veto_gamma1_E'}, path=roe_path)
variableToSignalSideExtraInfo('pi0:veto', {'daughter(1,cosTheta)': 'pi0veto_gamma1_cosTheta'}, path=roe_path)
variableToSignalSideExtraInfo('pi0:veto', {'daughter(1,clusterE1E9)': 'pi0veto_gamma1_clusterE1E9'}, path=roe_path)
variableToSignalSideExtraInfo('pi0:veto', {'daughter(1,clusterE9E21)': 'pi0veto_gamma1_clusterE9E21'}, path=roe_path)
variableToSignalSideExtraInfo('pi0:veto', {'daughter(1,clusterTiming)': 'pi0veto_gamma1_clusterTiming'}, path=roe_path)
variableToSignalSideExtraInfo('pi0:veto', {'daughter(1,clusterAbsZernikeMoment40)': 'pi0veto_gamma1_clusterAbsZernikeMoment40'}, path=roe_path)
variableToSignalSideExtraInfo('pi0:veto', {'daughter(1,clusterAbsZernikeMoment51)': 'pi0veto_gamma1_clusterAbsZernikeMoment51'}, path=roe_path)
variableToSignalSideExtraInfo('pi0:veto', {'daughter(1,clusterSecondMoment)': 'pi0veto_gamma1_clusterSecondMoment'}, path=roe_path)
variableToSignalSideExtraInfo('pi0:veto', {'mcPDG': 'pi0veto_mcPDG'}, path=roe_path)
variableToSignalSideExtraInfo('pi0:veto', {'cosTheta': 'pi0veto_cosTheta'}, path=roe_path)

variableToSignalSideExtraInfo('eta:veto2', {'M': 'eta0veto_M'}, path=roe_path)
variableToSignalSideExtraInfo('eta:veto2', {'daughter(0,E)': 'eta0veto_gamma0_E'}, path=roe_path)
variableToSignalSideExtraInfo('eta:veto2', {'daughter(1,E)': 'eta0veto_gamma1_E'}, path=roe_path)
variableToSignalSideExtraInfo('eta:veto2', {'daughter(1,cosTheta)': 'eta0veto_gamma1_cosTheta'}, path=roe_path)
variableToSignalSideExtraInfo('eta:veto2', {'daughter(1,clusterE1E9)': 'eta0veto_gamma1_clusterE1E9'}, path=roe_path)
variableToSignalSideExtraInfo('eta:veto2', {'daughter(1,clusterE9E21)': 'eta0veto_gamma1_clusterE9E21'}, path=roe_path)
variableToSignalSideExtraInfo('eta:veto2', {'daughter(1,clusterTiming)': 'eta0veto_gamma1_clusterTiming'}, path=roe_path)
variableToSignalSideExtraInfo('eta:veto2', {'daughter(1,clusterAbsZernikeMoment40)': 'eta0veto_gamma1_clusterAbsZernikeMoment40'}, path=roe_path)
variableToSignalSideExtraInfo('eta:veto2', {'daughter(1,clusterAbsZernikeMoment51)': 'eta0veto_gamma1_clusterAbsZernikeMoment51'}, path=roe_path)
variableToSignalSideExtraInfo('eta:veto2', {'daughter(1,clusterSecondMoment)': 'eta0veto_gamma1_clusterSecondMoment'}, path=roe_path)
variableToSignalSideExtraInfo('eta:veto2', {'mcPDG': 'eta0veto_mcPDG'}, path=roe_path)
variableToSignalSideExtraInfo('eta:veto2', {'cosTheta': 'eta0veto_cosTheta'}, path=roe_path)
analysis_main.for_each('RestOfEvent', 'RestOfEvents', roe_path)

# VETO ends here
# ----------------
#'''


toolsB0_meson =  ['Kinematics','^B0 -> [^Xsd -> ^pi+ ^pi- [ ^K_S0 ->  ^pi+ ^pi- ] ] ^gamma']
toolsB0_meson += ['CustomFloats[cosTheta:isSignal:isContinuumEvent:myRating:myRatingCriteria]', '^B0']
toolsB0_meson += ['CustomFloats[pi0veto_M:pi0veto_gamma0_E:pi0veto_gamma1_E:pi0veto_mcPDG:pi0veto_cosTheta:pi0veto_gamma1_cosTheta:pi0veto_gamma1_clusterE1E9:pi0veto_gamma1_clusterE9E21:pi0veto_gamma1_clusterTiming:pi0veto_gamma1_clusterAZM40:pi0veto_gamma1_clusterAZM51:pi0veto_gamma1_clusterSecondMoment]', '^B0']
toolsB0_meson += ['CustomFloats[eta0veto_M:eta0veto_gamma0_E:eta0veto_gamma1_E:eta0veto_mcPDG:eta0veto_cosTheta:eta0veto_gamma1_cosTheta:eta0veto_gamma1_clusterE1E9:eta0veto_gamma1_clusterE9E21:eta0veto_gamma1_clusterTiming:eta0veto_gamma1_clusterAZM40:eta0veto_gamma1_clusterAZM51:eta0veto_gamma1_clusterSecondMoment]', '^B0']
#toolsB0_meson += ['MCKinematics','^B0 ->  ^Xsd gamma']
toolsB0_meson += ['MCTruth','^B0 -> [^Xsd -> ^pi+ ^pi- ^K_S0] ^gamma']
toolsB0_meson += ['MCHierarchy','^B0 -> [Xsd -> ^pi+ ^pi- ^K_S0] ^gamma']
toolsB0_meson += ['Vertex','^B0 -> [Xsd -> pi+ pi- ^K_S0] gamma']
toolsB0_meson += ['CustomFloats[significanceOfDistance]', '^B0 -> [Xsd -> pi+ pi- ^K_S0] gamma']
toolsB0_meson += ['MCVertex','^B0 -> [Xsd -> pi+ pi- ^K_S0] gamma']
toolsB0_meson += ['InvMass','^B0 -> [^Xsd -> pi+ pi- ^K_S0]  gamma']
toolsB0_meson += ['DeltaEMbc','^B0']
toolsB0_meson += ['PID','B0 -> [Xsd -> ^pi+ ^pi- [ K_S0 ->  ^pi+ ^pi- ] ] gamma']
toolsB0_meson += ['CustomFloats[chiProb]','B0 -> [Xsd -> ^pi+ ^pi- [ K_S0 ->  ^pi+ ^pi- ] ] gamma']
toolsB0_meson += ['CustomFloats[d0:z0:cosTheta:isSignal]', 'B0 -> [Xsd -> ^pi+ ^pi- ^K_S0] ^gamma']
toolsB0_meson += ['CustomFloats[clusterMergedPi0:clusterSecondMoment:clusterErrorTiming:clusterTiming:clusterE1E9:clusterE9E21:clusterAbsZernikeMoment40:clusterAbsZernikeMoment51]', 'B0 -> [Xsd -> pi+ pi- K_S0] ^gamma']
toolsB0_meson += ['CustomFloats[useCMSFrame(daughterAngleInBetween(0,1)):cosHelicityAngle]', 'B0 -> [^Xsd -> pi+ pi- K_S0] gamma']
toolsB0_meson += ['Dalitz', '^B0 -> [Xsd -> ^pi+ ^pi- ^K_S0] gamma']
toolsB0_meson += ['TrackHits','B0 -> [Xsd -> ^pi+ ^pi-  [ K_S0 ->  ^pi+ ^pi- ] ] gamma']
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

K0starInfo =  ['Kinematics','^Xsd ->  ^pi+ ^pi- ^K_S0']
K0starInfo += ['MCKinematics','^Xsd']
K0starInfo += ['MCTruth','^Xsd']
K0starInfo += ['InvMass','^Xsd']
K0starInfo += ['Vertex','^Xsd']

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
#ntupleTree('K1Signal', 'Xsd:all', K0starInfo)
# Process the events
process(analysis_main)

# print out the summary
print(statistics)
