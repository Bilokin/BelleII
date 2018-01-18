from basf2 import *
from modularAnalysis import *
from vertex import *

from stdV0s import stdKshorts
from stdPhotons import *
from stdCharged import *
from stdFSParticles import *
#from flavorTagger import *

from beamparameters import *
# check if the required input file exists (from B2A101 example)
import os.path
import sys
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
use_central_database("GT_gen_prod_003.11_release-00-09-01-FEI-a")

# load input ROOT file2
add_beamparameters(analysis_main,'Y4S')
inputMdst('default', inputFilename)
#stdMu('all')
stdKshorts()
stdK('95eff')
stdPhotons('loose')
stdPi0s()
stdPi('all')
applyCuts('gamma:loose','1.4 < E < 4')
#applyCuts('K_S0:all','daughter(0,piid)>0.1 and daughter(1,piid)>0.1 and daughter(0,eid)<0.9 and daughter(1,eid)<0.9 and daughter(0,prid)<0.9 and daughter(1,prid)<0.9 and daughter(0,muid)<0.9 and daughter(1,muid)<0.9')
#vertexRave('K_S0:all',0.01)
vertexKFit('K_S0:all',0.0)
#reconstructDecay("K_10:all -> pi+:all pi-:all K_S0:all", "0.5 < M < 2")
#reconstructDecay("B0:signal -> K_10:all gamma:loose", " 4 < M < 6 and Mbc > 5.27 and deltaE < 0.1 and deltaE > -0.2")
#vertexRave('B0:signal',0.01, 'B0 -> [K_10 -> ^pi+ ^pi- ^K_S0] gamma')
reconstructDecay('rho+:signal -> pi+:all pi0:veryLoose','0.7 < M < 0.9')
reconstructDecay('D0:signal -> K-:95eff pi+:all',' 1.8< M < 1.9')
reconstructDecay('D*0:signal -> D0:signal pi0:veryLoose', '1.9 < M < 2.1')
#buildRestOfEvent('B0:signal')

oldMask = ('oldMask', 'useCMSFrame(p)<=3.2', 'p >= 0.05 and useCMSFrame(p)<=3.2')

# define the "cleaner" mask
#eclCut = '[E > 0.062 and abs(clusterTiming) < 18 and clusterReg==1] or \
#[E>0.060 and abs(clusterTiming) < 20 and clusterReg==2] or \
#[E>0.056 and abs(clusterTiming) < 44 and clusterReg==3]'
#cleanMask = ('cleanMask', 'abs(d0) < 10.0 and abs(z0) < 20.0', eclCut)

# append both masks to ROE
#appendROEMasks('B0:signal', [oldMask,cleanMask])

# choose one mask which is applied
#buildContinuumSuppression('B0:signal', 'cleanMask')

matchMCTruth('D0:signal')
matchMCTruth('D*0:signal')

#TagV('B0:signal', 'breco')

#flavorTagger(particleLists = 'B0:signal', weightFiles='B2JpsiKs_muBGx1')
#matchMCTruth('pi+:all')
#matchMCTruth('K_10:all')
#matchMCTruth('gamma:loose')
#rankByHighest()

toolsD0_meson = ['Kinematics','^D*0 -> [D0 -> ^K- ^pi+] ^pi0']
toolsD0_meson += ['MCTruth','^D*0 -> [D0 -> ^K- ^pi+] ^pi0']
toolsD0_meson += ['InvMass','^D*0 -> [^D0 -> K- pi+] ^pi0'] 
toolsD0_meson += ['DeltaEMbc','^D*0']


toolsB0_meson =  ['Kinematics','^B0 -> [^K_10 -> ^pi+ ^pi- ^K_S0] ^gamma']
toolsB0_meson += ['CustomFloats[cosTheta:isSignal:isContinuumEvent]', '^B0']
#toolsB0_meson += ['MCKinematics','^B0 ->  ^K_10 gamma']
toolsB0_meson += ['MCTruth','^B0 -> [^K_10 -> ^pi+ ^pi- ^K_S0] ^gamma']
toolsB0_meson += ['MCHierarchy','^B0 -> [K_10 -> ^pi+ ^pi- ^K_S0] gamma']
toolsB0_meson += ['Vertex','^B0 -> [K_10 -> pi+ pi- ^K_S0] gamma']
toolsB0_meson += ['MCVertex','^B0 -> [K_10 -> pi+ pi- ^K_S0] gamma']
#toolsB0_meson += ['PDGCode','^B0']
toolsB0_meson += ['InvMass','^B0 -> [^K_10 -> pi+ pi- ^K_S0]  gamma']
toolsB0_meson += ['DeltaEMbc','^B0']
#toolsB0_meson += ['PID','B0 -> [K_10 -> ^pi+ ^pi- K_S0] gamma']
#toolsB0_meson += ['MCReconstructible', 'B0 -> [K_10 -> ^pi+ ^pi- K_S0] gamma']
toolsB0_meson += ['CustomFloats[d0:z0:cosTheta:isSignal]', 'B0 -> [K_10 -> ^pi+ ^pi- ^K_S0] ^gamma']
toolsB0_meson += ['CustomFloats[useCMSFrame(daughterAngleInBetween(0,1)):cosHelicityAngle]', 'B0 -> [^K_10 -> pi+ pi- K_S0] gamma']
toolsB0_meson += ['Dalitz', '^B0 -> [K_10 -> ^pi+ ^pi- ^K_S0] gamma']
toolsB0_meson += ['TrackHits','B0 -> [K_10 -> ^pi+ ^pi- K_S0] gamma']
toolsB0_meson += ['TagVertex', '^B0']
toolsB0_meson += ['MCTagVertex', '^B0']
toolsB0_meson += ['DeltaT', '^B0']
toolsB0_meson += ['DeltaTErr', '^B0']
toolsB0_meson += ['MCDeltaT', '^B0']
#toolsB0_meson += ['FlavorTagging[TMVA-FBDT, FANN-MLP, qrCategories]', '^B0']
toolsB0_meson += ['FlavorTagging', '^B0']
toolsB0_meson += ['ContinuumSuppression', '^B0:phiKs']

toolsB0_meson += ['MassBeforeFit', '^B0']
toolsB0_meson += ['ROEMultiplicities', '^B0']



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
#K0Info += ['CustomFloats[distance]', '^K_S0']
K0Info += ['CustomFloats[cosTheta]', '^K_S0 -> ^pi+ ^pi-']
#K0Info += ['CustomFloats[nDaughters]', '^K_S0']
K0Info += ['TrackHits','K_S0 -> ^pi+ ^pi-']
K0Info += ['CustomFloats[d0:z0:d0Err:z0Err]', 'K_S0 -> ^pi+ ^pi-']


K0starInfo =  ['Kinematics','^K_10 ->  ^pi+ ^pi- ^K_S0']
K0starInfo += ['MCKinematics','^K_10']
K0starInfo += ['MCTruth','^K_10']
K0starInfo += ['InvMass','^K_10']
K0starInfo += ['Vertex','^K_10']

ntupleFile(outputFilename)
ntupleTree('D0Signal', 'D*0:signal', toolsD0_meson)
#ntupleTree('pions', 'pi+:all', PiInfo)
#ntupleTree('K0Signal', 'K_S0:all', K0Info)
#ntupleTree('GammaSignal', 'gamma:loose', gammaInfo)
#ntupleTree('K1Signal', 'K_10:all', K0starInfo)
# Process the events
process(analysis_main)

# print out the summary
print(statistics)
