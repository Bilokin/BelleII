
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


reconstructDecay('K*0:lse -> K_S0:all pi+:loose pi-:loose','0.8 < M < 1.1')
reconstructDecay('B0:m -> K*0:lse gamma:tight','4 < M < 6')


#vertexFit('K_S0:all',0)
#vertexFit('B0:m', 0, 'B0 -> [K*0 -> ^K_S0 ^pi+ ^pi-] ^gamma')
vertexRave('K_S0:all',0)
vertexRave('B0:m', 0, 'B0 -> [K*0 -> ^K_S0 ^pi+ ^pi-] ^gamma')

# Associates the MC truth to the reconstructed B0-K*0-gamma
matchMCTruth('gamma:tight')
matchMCTruth('K_S0:all')
matchMCTruth('pi+:loose')
matchMCTruth('B0:m')

#matchMCTruth('pi-:all')
printDataStore()
#Create list of variable associated to gamma, pion and Kaons
#These lists will fill one Ntuplefile with several Ntupletree

#toolsRelated = ['Kinematics', 'B0 -> [K*0 -> ^K_S0 ^pi+ ^pi-] ^gamma']

toolsB0_meson =  ['Kinematics','^B0']
toolsB0_meson += ['MCKinematics','^B0']
toolsB0_meson += ['MCTruth','^B0']
toolsB0_meson += ['MCHierarchy','^B0']
toolsB0_meson += ['PDGCode','^B0']
toolsB0_meson += ['InvMass','^B0']
toolsB0_meson += ['DeltaEMbc','^B0']
toolsB0_meson += ['Vertex','^B0']
toolsB0_meson += ['MCVertex','^B0']
toolsB0_meson += ['CustomFloats[distance:deltaE:isSignal:daughter(0,mcPDG):daughter(1,mcPDG):E:cosTheta:genMotherPDG]','^B0']
#toolsB0_meson += ['CustomFloats[daughter(0,mcPDG):daughter(1,mcPDG):E:cosTheta:genMotherPDG]' '^B0']




toolsGamma =  ['Kinematics', '^gamma']

#toolsK_S0  += ['MCHierarchy', 'B0 -> [K*0 -> ^K_S0 pi+ pi-] gamma']
#toolsGamma =  ['Kinematics', '^gamma']
toolsGamma += ['MCKinematics', '^gamma']
toolsGamma += ['MCTruth', '^gamma']
toolsGamma += ['MCHierarchy', '^gamma']
toolsGamma += ['PDGCode', '^gamma']
#toolsGamma += ['ExtraEnergy', '^gamma']
toolsGamma += ['CustomFloats[goodGamma:E:cosTheta:genMotherPDG:isExtendedSignal:isPrimarySignal:isSignal:mcErrors:mcInitial:isPrimarySignal:mcVirtual:nMCMatches:E:p:NumberOfMCParticlesInEvent(-511)]', '^gamma']

toolsGamma += ['Cluster', '^gamma']
toolsGamma += ['RecoStats', '^gamma']

toolsK_S0  = ['EventMetaData', '^K_S0']
#toolsK_S0   = ['CMSKinematics', '^K_S0']
toolsK_S0  += ['Kinematics', '^K_S0']
toolsK_S0  += ['MCKinematics', '^K_S0']
toolsK_S0  += ['MCGenKinematics[200]', '^K_S0']
toolsK_S0  += ['MCTruth', '^K_S0']
#toolsK_S0  += ['MCHierarchy', '^K_S0']
toolsK_S0  += ['MCHierarchy', '^K_S0']
toolsK_S0  += ['PDGCode', '^K_S0']
toolsK_S0  += ['Vertex', '^K_S0']
toolsK_S0  += ['MCVertex', '^K_S0']
#toolsK_S0  += ['CustomFloats[E:p:K_vs_piid:Kid_belle:cosTheta:pt:x:y:z:mcDecayTime]', '^K_S0']
#toolsK_S0  += ['InvMass', '^K_S0']
toolsK_S0  += ['PID', '^K_S0']
toolsK_S0  += ['MCReconstructible', '^K_S0']
toolsK_S0  += ['Track', '^K_S0']
toolsK_S0  += ['InvMass', '^K_S0']
toolsK_S0  += ['CustomFloats[distance:E:p:K_vs_piid:Kid_belle:cosTheta:pt:x:y:z:mcDecayTime:isSignal:Mbvf:InvM]', '^K_S0']

#toolsFlight  = ['FlightInfo', '^K*0 -> ^K_S0']
#toolsFlight  += ['MCFlightInfo', '^K*0 -> ^K_S0']


toolsPi_plus   = ['Kinematics', '^pi+']
toolsPi_plus  += ['MCKinematics', '^pi+']
toolsPi_plus  += ['MCTruth', '^pi+']
toolsPi_plus  += ['MCHierarchy', '^pi+']
toolsPi_plus  += ['PDGCode', '^pi+']
toolsPi_plus  += ['InvMass', '^pi+']
toolsPi_plus  += ['CustomFloats[mcP:piid:cosTheta:pt:p:E:mcE:chiProb]', '^pi+']


#toolsPi_minus   = ['Kinematics', '^pi-']
#toolsPi_minus  += ['MCKinematics', '^pi-']
#toolsPi_minus  += ['MCTruth', '^pi-']
#toolsPi_minus  += ['MCHierarchy', '^pi-']
#toolsPi_minus  += ['PDGCode', '^pi-']


#Creation of Ntuple
ntupleFile('5000_all-photons_Reco_B0_v3_test.root')
#ntupleFile('deafult1_5000_all-photons_reco_new_v4.root')
#ntupleFile('default2_5000_all-photons_reco_new_v4.root')


ntupleTree('phot_5000', 'gamma:tight', toolsGamma)
ntupleTree('Kaons_S', 'K_S0:all', toolsK_S0)
ntupleTree('pions_p', 'pi+:loose', toolsPi_plus)
#ntupleTree('pions_m', 'pi-:all', toolsPi_minus)
ntupleTree('b0', 'B0:m', toolsB0_meson)
#ntupleTree('Flight', 'K*0', toolsFlight)

# Process the events
process(analysis_main)

# print out the summary
print(statistics)
                                                      
