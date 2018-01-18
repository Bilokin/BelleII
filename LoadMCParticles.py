#!/usr/bin/env python3
# -*- coding: utf-8 -*-

#######################################################
# This tutorial demonstrates how to load generated final
# state particles (MCParticle objects) as Particles and
# create ParticleLists for each final state particle
# type: gamma/e/mu/pi/K/proton/K_L.
#
# All analysis modules and tools (e.g. making combinations, ...)
# have interfaces for ParticleLists so this step is
# neccessary if analysis tools are to be used.
#
# Contributors: A. Zupanc (June 2014)
#
######################################################

from basf2 import *
from modularAnalysis import *

defaultInputFilename = "evtgen.root"
defaultInputFoldername = "test"
inputFilename = defaultInputFoldername + '/' + defaultInputFilename
defaultOutputFilename = "reco-tf2signal.root"
defaultOutputFoldername = "test"
outputFilename = defaultOutputFoldername + '/' + defaultOutputFilename
for arg in sys.argv:
	print(arg)
if len(sys.argv)==2:
	inputFilename = sys.argv[1]
if len(sys.argv)==3:
	inputFilename = sys.argv[1]
	outputFilename = sys.argv[2]

# load input ROOT file
inputMdst('None', inputFilename)

# print contents of the DataStore before loading MCParticles
printDataStore()

# create and fill gamma/e/mu/pi/K/p ParticleLists
# second argument are the selection criteria: '' means no cut, take all
photons = ('gamma:gen', '')
electrons = ('e-:gen', '')
muons = ('mu-:gen', '')
pionsM = ('pi-:gen', '')
pions0 = ('pi0:gen', '')
kaons = ('K_S0:gen', '')
kaonsS = ('K_10:gen', '')
protons = ('anti-p-:gen', '')
b0s = ('B0:gen', '')

kshortcut = "abs(daughter(0,mcPDG)) == 211 "
b0cut = "countDaughters( ) == 2  and abs(daughter(1,mcPDG)) == 22"
ks0cut = "countDaughters( ) == 2 and abs(daughter(1,mcPDG)) == 22 and abs(daughter(0,mcPDG)) == 310"
gammacut = "E > 1.5  and abs(genMotherPDG) == 511"

fillParticleListsFromMC([electrons, muons, pionsM, pions0])

fillParticleListsFromMC(("Upsilon(4S):gen",''),True)
fillParticleListsFromMC(("K_10:gen",'abs(genMotherPDG) == 511'),True)
fillParticleListsFromMC(("K_S0:gen",kshortcut),True)
fillParticleListsFromMC(("B0:gen", b0cut),True)
fillParticleListsFromMC(("K*0:gen", ks0cut),True)
fillParticleListsFromMC(("gamma:gen", gammacut),True)

#applyCuts('K_S0:my','abs(genMotherPDG) == 10313 or abs(genMotherPDG) == 323')

applyCuts('gamma:gen','abs(genMotherPDG) == 511')
#applyCuts('pi-:gen','abs(genMotherPDG) == 323 or abs(genMotherPDG) == 113 or abs(genMotherPDG) == 10313')

applyCuts('K_10:gen','abs(mcPDG) == 10313 and abs(genMotherPDG) == 511 and daughter(0,mcPDG) == 223')
reconstructDecay('B0:genSig -> K_S0:gen pi+:gen pi-:gen gamma:gen', '5.<M<5.5')

matchMCTruth('B0:genSig')
printVariableValues('B0:genSig',['p','px','mcPDG','genMotherPDG' ,'nDaughters']) 
applyCuts('B0:genSig','abs(mcPDG) == 511')
toolsKS0 = ['Kinematics', '^K*0 -> ^K_S0 ^gamma']
toolsKS0 += ['InvMass', '^K*0 -> ^K_S0 gamma']
toolsKS0 += ['MCTruth', '^K*0 -> ^K_S0 ^gamma']

toolsK0my = ['Kinematics', '^K_S0 -> ^pi+ ^pi-']
toolsK0my += ['InvMass', '^K_S0']
toolsK0my += ['Vertex', '^K_S0']
toolsK0my += ['MCVertex', '^K_S0']
toolsK0my += ['CustomFloats[cosTheta]', '^K_S0  -> ^pi+ ^pi-']

toolsK0p = ['Kinematics', '^K_10']
toolsK0p += ['InvMass', '^K_10']
toolsK0p += ['CustomFloats[cosTheta]', '^K_10']
toolsK0p += ['MCTruth', '^K_10']
toolsK0p += ['MCVertex', '^K_10']
toolsK0p += ['CustomFloats[abs(genMotherPDG)]', '^K_10']

toolsGamma = ['Kinematics', '^gamma -> e+ e-']
toolsGamma += ['CustomFloats[cosTheta]', '^gamma']
toolsGamma += ['CustomFloats[genMotherPDG]', '^gamma']

toolsB0 = ['Kinematics', '^B0 -> ^K_10 ^gamma']
toolsB0 += ['EventMetaData', '^B0']
toolsB0 += ['CMSKinematics', '^B0']
toolsB0 += ['InvMass', '^B0']
toolsB0 += ['MCVertex', '^B0']
toolsB0 += ['CustomFloats[daughter(0,mcPDG):daughter(1,mcPDG)]', 'B0 -> ^K_10 gamma']
toolsB0 += ['CustomFloats[cosTheta]', '^B0 -> ^K_10 ^gamma']
toolsB0 += ['CustomFloats[nDaughters]', '^B0 -> ^K_10 ^gamma']

toolsB0SIG = ['Kinematics', '^B0 -> ^K_S0 ^pi+ ^pi- ^gamma']
toolsB0SIG += ['InvMass', '^B0']
toolsB0SIG += ['MCTruth', '^B0']
toolsB0SIG += ['MCVertex', '^B0 -> ^K_S0 ^pi+ ^pi- gamma']
toolsB0SIG += ['Dalitz', '^B0 -> ^K_S0 ^pi+ ^pi- gamma']
toolsB0SIG += ['MCHierarchy', 'B0 -> ^K_S0 pi+ pi- gamma']
toolsB0SIG += ['CustomFloats[daughterInvariantMass(0, 1, 2)]', '^B0 -> K_S0 pi+ pi- gamma']
toolsB0SIG += ['CustomFloats[cosTheta:phi]', '^B0']
toolsB0SIG += ['CustomFloats[nDaughters]', '^B0']
toolsB0SIG += ['CustomFloats[daughter(0,E)]', '^B0']
toolsB0SIG += ['CustomFloats[daughter(1,E)]', '^B0']
toolsB0SIG += ['CustomFloats[daughter(2,E)]', '^B0']
toolsB0SIG += ['CustomFloats[cosTheta:phi:E]', 'B0 -> ^K_S0 ^pi+ ^pi- ^gamma']

toolsB0SIG += ['CustomFloats[daughter(0,px)]', '^B0']
toolsB0SIG += ['CustomFloats[daughter(1,px)]', '^B0']
toolsB0SIG += ['CustomFloats[daughter(2,px)]', '^B0']
toolsB0SIG += ['CustomFloats[daughter(0,py)]', '^B0']
toolsB0SIG += ['CustomFloats[daughter(1,py)]', '^B0']
toolsB0SIG += ['CustomFloats[daughter(2,py)]', '^B0']
toolsB0SIG += ['CustomFloats[daughter(0,pz)]', '^B0']
toolsB0SIG += ['CustomFloats[daughter(1,pz)]', '^B0']
toolsB0SIG += ['CustomFloats[daughter(2,pz)]', '^B0']

toolsY4S  = ['Kinematics', '^Upsilon(4S) -> ^B0 ^anti-B0']
toolsY4S += ['CMSKinematics', '^Upsilon(4S) ->  ^B0 ^anti-B0']
toolsY4S += ['InvMass', '^Upsilon(4S)']
toolsY4S += ['MCVertex', '^Upsilon(4S) -> ^B0 ^anti-B0']
toolsY4S += ['MCDeltaT', 'Upsilon(4S) -> ^B0 ^anti-B0']
ntupleFile(outputFilename)
ntupleTree('gamma', 'gamma:gen', toolsGamma)
ntupleTree('B0s', 'B0:gen', toolsB0)
ntupleTree('Kstar0', 'K*0:gen', toolsKS0)
ntupleTree('B0sSIG', 'B0:genSig', toolsB0SIG)
ntupleTree('K0s', 'K_S0:gen', toolsK0my)
ntupleTree('K0sp', 'K_10:gen', toolsK0p)
ntupleTree('Y4S', 'Upsilon(4S):gen', toolsY4S)
# Process the events
process(analysis_main)

# print out the summary
print(statistics)
