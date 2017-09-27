
from basf2 import *
from modularAnalysis import *
# check if the required input file exists (from B2A101 example)
import os.path
import sys
from vertex import vertexRave
from variables import variables
variables.addAlias('Mbvf', 'extraInfo(massBeforeFit)')
from beamparameters import *
from stdCharged import *
from stdLightMesons import *
from stdPhotons import *
from stdV0s import *
#import MCDecayString
#if not os.path.isfile('B-K*0gamma-reco.root'):
#   sys.exit('Required input file (B-K*0gamma-reco.root) does not exist. '
#           'Please run EvtGeneration.py tutorial script first.')
add_beamparameters(analysis_main,'Y4S')
# load input ROOT file
#inputMdst('default', '5000-B-K*0gamma-reco.root')

#inputMdst('default', '5000-B-K*0gamma-reco_v3_test.root')
inputMdst('default', 'reco-signal.root')
#inputMdst('None', 'default_5000-B-K*0gamma-reco.root')
#inputMdst('default', 'default_5000-B-K*0gamma-reco.root')

#Print contents of the DataStore before loading MCParticles
#printDataStore()


# create and fill gamma/pi/K_S0 ParticleLists
# second argument are the selection criteria: '' means no cut, take all
#photons = ('gamma:all', '')
#kaons = ('K_S0:all', '')
#pions_plus = ('pi+:all', '')
#pions_minus = ('pi-:all', '')
#b0meson = ('B0:m', '4 < M < 6')

stdPhotons('tight')
stdLoosePi()
#stdLooseK()
stdKshorts()
#fillParticleListsFromMC([b0meson])

#fillParticleListsFromMC([photons, pions_plus, kaons, b0meson])
#fillParticleLists([photons, pions_plus, kaons])
#fillParticleLists(mettre decay)
#variablesToExtraInfo('K_S0:all',{'M':'massBeforeFit'})

# print content of the DataStore after loading MCParticles
# the difference is that DataStore now contains StoreArray<Particle>
# filled with Particles created from generated final state particles
#
#printDataStore()
# print charge, energy and total momentum of generated kaons
# and x,y,z components of momenta for generated pions
#printList('gamma:all', False)
# the list of all available variables can be obtained by executing
# basf2 analysis/scripts/variables.py
#
#printVariableValues('gamma:all', ['E', 'p'])
#printPrimaryMCParticles()


reconstructDecay('K*0:all -> K_S0:all pi+:loose pi-:loose','0.8 < M < 1.1')
reconstructDecay('B0:signal -> K*0:all gamma:tight','4 < M < 6')


#vertexFit('K_S0:all',0)
#vertexFit('B0:m', 0, 'B0 -> [K*0 -> ^K_S0 ^pi+ ^pi-] ^gamma')
vertexRave('K_S0:all',0)
vertexRave('B0:signal', 0, 'B0 -> [K*0 -> ^K_S0 ^pi+ ^pi-] ^gamma')

# Associates the MC truth to the reconstructed B0-K*0-gamma
matchMCTruth('gamma:tight')
matchMCTruth('K_S0:all')
matchMCTruth('pi+:loose')
matchMCTruth('B0:signal')

#matchMCTruth('pi-:all')
printDataStore()
#Create list of variable associated to gamma, pion and Kaons
#These lists will fill one Ntuplefile with several Ntupletree

#toolsRelated = ['Kinematics', 'B0 -> [K*0 -> ^K_S0 ^pi+ ^pi-] ^gamma']

toolsB0_meson =  ['Kinematics','^B0 -> ^K*0 gamma']
toolsB0_meson += ['CustomFloats[cosTheta]', '^B0']
toolsB0_meson += ['MCKinematics','^B0 ->  ^K*0 gamma']
toolsB0_meson += ['MCTruth','^B0 -> ^K*0 gamma']
toolsB0_meson += ['MCHierarchy','^B0 -> ^K*0  gamma']
toolsB0_meson += ['Vertex','B0 -> ^K*0  gamma']
toolsB0_meson += ['PDGCode','^B0']
toolsB0_meson += ['InvMass','^B0 -> ^K*0  gamma']
toolsB0_meson += ['DeltaEMbc','^B0']

gammaInfo =  ['Kinematics','^gamma']
gammaInfo += ['MCKinematics','^gamma']
gammaInfo += ['MCTruth','^gamma']
gammaInfo += ['MCHierarchy','^gamma']
gammaInfo += ['PDGCode','^gamma']
gammaInfo += ['InvMass','^gamma']
gammaInfo += ['DeltaEMbc','^gamma']

PiInfo  =  ['Kinematics','^pi+']
PiInfo  +=  ['MCKinematics','^pi+']
PiInfo  +=  ['MCKinematics','^pi+']
PiInfo += ['MCTruth','^pi+']

K0Info =  ['Kinematics','^K_S0 -> ^pi+ ^pi-']
K0Info += ['MCKinematics','^K_S0 -> ^pi+ ^pi-']
K0Info += ['MCTruth','^K_S0 -> ^pi+ ^pi-']
K0Info += ['InvMass','^K_S0']
K0Info += ['Vertex', '^K_S0']
K0Info += ['MCVertex', '^K_S0']
K0Info += ['CustomFloats[distance]', '^K_S0']
K0Info += ['CustomFloats[cosTheta]', '^K_S0 -> ^pi+ ^pi-']
K0Info += ['CustomFloats[nDaughters]', '^K_S0']
K0Info += ['CustomFloats[daughter(0,piid)]', '^K_S0']
K0Info += ['CustomFloats[daughter(1,piid)]', '^K_S0']
K0Info += ['CustomFloats[daughter(0,eid)]', '^K_S0']
K0Info += ['CustomFloats[daughter(1,eid)]', '^K_S0']
K0Info += ['MCReconstructible', 'K_S0 -> ^pi+ ^pi-']


K0starInfo =  ['Kinematics','^K*0 -> ^K_S0 ^pi+ ^pi- ']
K0starInfo += ['MCKinematics','^K*0']
K0starInfo += ['MCTruth','^K*0']
K0starInfo += ['InvMass','^K*0']
K0starInfo += ['Vertex','^K*0']

ntupleFile('test.root')
ntupleTree('B0Signal', 'B0:signal', toolsB0_meson)
ntupleTree('GammaSignal', 'gamma:tight', gammaInfo)
ntupleTree('K0Signal', 'K_S0:all', K0Info)
ntupleTree('K1Signal', 'K*0:all', K0starInfo)
ntupleTree('pions', 'pi+:all', PiInfo)
# Process the events
process(analysis_main)

# print out the summary
print(statistics)
