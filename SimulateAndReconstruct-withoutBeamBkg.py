#!/usr/bin/env python3
# -*- coding: utf-8 -*-

######################################################
# This tutorial demonstrates how to perform detector simulation
# and reconstruction (track finding+track fitting+ecl reconstruction+...)
# on a previously generated events.
#
# In this example no Beam Bakcground is mixed in. See
# B2A104-SimulateAndReconstruct-withBeamBkg.py to check
# how to mix beam background in the simulation.
#
# The processed events are saved to the output ROOT file that
# now contain in addition to the generated particles
# (MCParticle objects stored in the StoreArray<MCParticle>) also
# reconstructed MDST objects (Track/ECLCluster/KLMCluster/...).
# Contributors: A. Zupanc (June 2014)
#
######################################################

from basf2 import *
from modularAnalysis import inputMdst
from modularAnalysis import analysis_main
from simulation import add_simulation
from reconstruction import add_reconstruction
from reconstruction import add_mdst_output
from beamparameters import add_beamparameters
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

add_beamparameters(analysis_main, 'Y4S')

# check if the required input file exists (from B2A101 example)
#if not os.path.isfile('B2A101-Y4SEventGeneration-evtgen.root'):
#if not os.path.isfile('evtgen.root'):
#    sys.exit('Required input file (B2A101-Y4SEventGeneration-evtgen.root) does not exist. '
#             'Please run B2A101-Y4SEventGeneration.py tutorial script first.')

# load input ROOT file
#inputMdst('None', 'B2A101-Y4SEventGeneration-evtgen.root')
inputMdst('None', inputFilename)

# simulation
add_simulation(analysis_main, use_vxdtf2 = True)

# reconstruction
add_reconstruction(analysis_main, use_vxdtf2 = True)

# dump in MDST format
add_mdst_output(analysis_main, True, outputFilename)

# Show progress of processing
progress = register_module('ProgressBar')
analysis_main.add_module(progress)

# Process the events
process(analysis_main)

# print out the summary
print(statistics)
