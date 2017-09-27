
from basf2 import *
from modularAnalysis import *
from vertex import *

from stdV0s import stdKshorts
from stdPhotons import *
from stdCharged import *
from stdFSParticles import *

from beamparameters import *
# check if the required input file exists (from B2A101 example)
import os.path
import sys
if not os.path.isfile('reco-signal.root'):
    sys.exit('Required input file (B2A101-Y4SEventGeneration-gsim-BKGx0.root) does not exist. '
             'Please run B2A101-Y4SEventGeneration.py and B2A103-SimulateAndReconstruct-withoutBeamBkg.py '
             'tutorial scripts first.')

# load input ROOT file
add_beamparameters(analysis_main,'Y4S')
inputMdst('default', 'reco-signal.root')
stdPi('all')
stdPi('')
stdMu('all')
stdKshorts()
stdPhotons('loose')
stdPi0s()
#fillParticleList('K_S0:mdst','')
#stdFSParticles()

applyCuts('gamma:loose','1.6 < E < 4')
#applyCuts('K_S0:all','daughter(0,piid)>0.1 and daughter(1,piid)>0.1 and daughter(0,eid)<0.9 and daughter(1,eid)<0.9 and daughter(0,prid)<0.9 and daughter(1,prid)<0.9 and daughter(0,muid)<0.9 and daughter(1,muid)<0.9')
#applyCuts('K_S0:all','dr > 0.1 and 0.48 < M < 0.52')
#applyCuts('pi+:all','piid > 0.437')
#stdCharged()
#reconstructDecay("K*+:all ->^K_S0:all ^pi+:all", "0.5 < M < 1.2")
#reconstructDecay("rho0:all ->pi+:all pi-:all", "0.2 < M < 1.6")
#vertexRave('K_S0:all',0.01)
reconstructDecay("K_10:all -> pi+:all pi-:all K_S0:all", "0.5 < M < 2")
#applyCuts('K_10:all','daughter(0,piid)>0.1 and daughter(1,piid)>0.1 and daughter(0,eid)<0.9 and daughter(1,eid)<0.9 and daughter(0,prid)<0.9 and daughter(1,prid)<0.9 and daughter(0,muid)<0.9 and daughter(1,muid)<0.9')
reconstructDecay("B0:m -> K_10:all gamma:loose", " 4 < M < 6")# and Mbc > 5.2 and abs(deltaE) < 0.250")
vertexRave('B0:m',0.02, 'B0 -> [K_10 -> ^pi+ ^pi- ^K_S0] ^gamma')
#vertexKFit('B0:m',0.0);
#buildRestOfEvent('B0:m')

#reconstructDecay("B0:sig ->K_S0:all pi+:all pi-:all gamma:loose", " 4 < M < 6")
#matchMCTruth('K_S0:all')
matchMCTruth('pi+:all')
matchMCTruth('K_10:all')
matchMCTruth('B0:m')
matchMCTruth('gamma:loose')

#printVariableValues('gamma:loose',['E'])
#printVariableValues('B0:m',['M','dr','mcPDG'])
#printVariableValues('K_10:all',['M','dr','mcPDG'])
#printList('B0:m', True)

# print contents of the DataStore before loading Particles
#printDataStore()
toolsB0_meson =  ['Kinematics','^B0 -> [^K_10 -> ^pi+ ^pi- ^K_S0] gamma']
toolsB0_meson += ['CustomFloats[cosTheta]', '^B0 ->  ^K_10 gamma']
toolsB0_meson += ['MCKinematics','^B0 ->  ^K_10 gamma']
toolsB0_meson += ['MCTruth','^B0 -> ^K_10 gamma']
toolsB0_meson += ['MCHierarchy','^B0']
toolsB0_meson += ['Vertex','^B0']
toolsB0_meson += ['PDGCode','^B0']
toolsB0_meson += ['InvMass','^B0 -> [^K_10 -> pi+ pi- ^K_S0]  gamma']
toolsB0_meson += ['DeltaEMbc','^B0']
#toolsB0_meson += ['MCReconstructible', 'B0 -> [K_10 -> ^pi+ ^pi- K_S0] gamma']
toolsB0_meson += ['CustomFloats[d0:z0:cosTheta:isSignal]', 'B0 -> [K_10 -> ^pi+ ^pi- K_S0] gamma']
toolsB0_meson += ['Dalitz', '^B0 -> [K_10 -> ^pi+ ^pi- ^K_S0] gamma']
toolsB0_meson += ['TrackHits','B0 -> [K_10 -> ^pi+ ^pi- K_S0] gamma']

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
PiInfo += ['MCVertex','^pi+']
PiInfo += ['Track','^pi+']
PiInfo += ['TrackHits','^pi+']
PiInfo += ['PID','^pi+']


K0Info =  ['Kinematics','^K_S0 -> ^pi+ ^pi-']
K0Info += ['MCKinematics','^K_S0 -> ^pi+ ^pi-']
K0Info += ['MCTruth','^K_S0 -> ^pi+ ^pi-']
K0Info += ['InvMass','^K_S0']
K0Info += ['Vertex', '^K_S0']
K0Info += ['MCVertex', '^K_S0']
K0Info += ['CustomFloats[distance]', '^K_S0']
K0Info += ['CustomFloats[cosTheta]', '^K_S0 -> ^pi+ ^pi-']
K0Info += ['CustomFloats[nDaughters]', '^K_S0']
#K0Info += ['CustomFloats[daughter(0,piid)]', '^K_S0']
#K0Info += ['CustomFloats[daughter(1,piid)]', '^K_S0']
#K0Info += ['CustomFloats[daughter(0,eid)]', '^K_S0']
#K0Info += ['CustomFloats[daughter(1,eid)]', '^K_S0']
K0Info += ['CustomFloats[d0:z0:d0Err:z0Err]', 'K_S0 -> ^pi+ ^pi-']
K0Info += ['MCReconstructible', 'K_S0 -> ^pi+ ^pi-']


K0starInfo =  ['Kinematics','^K_10 ->  ^pi+ ^pi- ^K_S0']
K0starInfo += ['MCKinematics','^K_10']
K0starInfo += ['MCTruth','^K_10']
K0starInfo += ['InvMass','^K_10']
K0starInfo += ['Vertex','^K_10']

ntupleFile('test.root')
ntupleTree('B0Signal', 'B0:m', toolsB0_meson)
ntupleTree('GammaSignal', 'gamma:loose', gammaInfo)
ntupleTree('K0Signal', 'K_S0:all', K0Info)
ntupleTree('K1Signal', 'K_10:all', K0starInfo)
ntupleTree('pions', 'pi+:all', PiInfo)
# Process the events
process(analysis_main)

# print out the summary
print(statistics)
