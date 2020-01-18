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
import re
import basf2           as b2
import modularAnalysis as ma
import flavorTagger    as ft
import vertex          as vtx
import stdCharged as sc
import stdV0s     as sv
import stdPi0s    as sp
import stdPhotons as sg
import variables.collections as vc
import variables.utils as vu
import beamparameters as bp
ratingVar = "chiProb"
inputFilename = 'input.root' 
outputFilename = 'output.root'
def makePretty(varlist):
	output = []
	for var in varlist:
		newvar = re.sub('[\*\+:_(,)\-]', '', var)
		if (newvar != var):
			variables.addAlias(newvar, var)
		output+=[newvar]
	return output

from ROOT import Belle2
env = Belle2.Environment.Instance()
print(env.getInputFilesOverride())
print(env.getOutputFileOverride())
outputFilename = env.getOutputFileOverride()

b2.use_database_chain()
b2.use_central_database('release-03-01-00')        # Database for MC  
b2.use_central_database("analysis_tools_release-03-01-00") # Database with Flavor Tag parameters


from variables import variables
variables.addAlias('EW90','extraInfo(EW90)')
variables.addAlias('pMissCMS','extraInfo(pMissCMS)')
variables.addAlias('cosThetaMissCMS','extraInfo(cosThetaMissCMS)')
variables.addAlias('myRating','extraInfo(myRating)')
variables.addAlias('myRatingCriteria',ratingVar)
variables.addAlias('B0_CosTBTO','cosTBTO')
variables.addAlias('B0_cc2','CleoConeCS(2)')
variables.addAlias('B0_hso12','KSFWVariables(hso12)')
variables.addAlias('B0_R2','R2')
variables.addAlias('B0_hso10','KSFWVariables(hso10)')
variables.addAlias('B0_hso14','KSFWVariables(hso14)')
variables.addAlias('B0_cc9','CleoConeCS(9)')
variables.addAlias('B0_CosTBz','cosTBz')
variables.addAlias('B0_ThrustO','thrustOm')
variables.addAlias('B0_ThrustB','thrustBm')
variables.addAlias('B0_cc4','CleoConeCS(4)')
variables.addAlias('B0_hso02','KSFWVariables(hso02)')
#variables.addAlias('MCTagBFlavor','internalTagVMCFlavor')
variables.addAlias('CSMVA','extraInfo(CSMVA)')
variables.addAlias('XsdM','daughterInvM(0,1,2)')
variables.addAlias('pi0Likeness','extraInfo(Pi0_Prob)')
variables.addAlias('etaLikeness','extraInfo(Eta_Prob)')
variables.addAlias('cosThetaCMS','useCMSFrame(cosTheta)')
variables.addAlias('pCMS','useCMSFrame(p)')
variables.addAlias('ECMS','useCMSFrame(E)')
variables.addAlias('m12','daughterInvariantMass(0,1)')
variables.addAlias('m13','daughterInvariantMass(0,2)')
variables.addAlias('m23','daughterInvariantMass(1,2)')

main_path = b2.create_path() # Declaration of main path
bp.add_beamparameters(main_path,'Y4S')
ma.inputMdst('MC10', inputFilename, path=main_path)
sv.goodBelleKshort(path=main_path)
sg.stdPhotons('loose', path=main_path)
sc.stdPi('95eff', path=main_path)
ma.applyCuts('gamma:loose','1.4 < E < 4', path=main_path)
krescuts = " and daughterInvM(0,1,2) < 2 and daughterInvM(0,1) > 0.6 and daughterInvM(0,1) < 0.9"
#reconstructDecay(Kres+":all -> pi+:good pi-:good K_S0:all", krescuts)
ma.reconstructDecay("B0:signal -> pi+:95eff pi-:95eff K_S0:legacyGoodKS gamma:loose",
        "Mbc > 5.2 and deltaE < 0.2 and deltaE > -0.2 and  -0.65 < daughter(1, cosTheta) < 0.85"+krescuts, path=main_path)
ma.vertexRave('B0:signal',0.0001, 'B0 -> ^pi+ ^pi- ^K_S0 gamma', path=main_path)
#vertexTree('B0:signal',0.0001)

ma.rankByHighest('B0:signal',ratingVar, 1, outputVariable='myRating', path=main_path)

ma.buildRestOfEvent('B0:signal', path=main_path)


# define the "cleaner" mask
eclCut = '[E > 0.062 and abs(clusterTiming) < 18 and clusterReg==1] or \
[E>0.060 and abs(clusterTiming) < 20 and clusterReg==2] or \
[E>0.056 and abs(clusterTiming) < 44 and clusterReg==3]'
cleanMask = ('cleanMask', 'abs(d0) < 10.0 and abs(z0) < 20.0', eclCut)

# append both masks to ROE
ma.appendROEMasks('B0:signal', [cleanMask], path=main_path)

# choose one mask which is applied
ma.buildContinuumSuppression('B0:signal', 'cleanMask', path=main_path)

ma.matchMCTruth('B0:signal', path=main_path)

vtx.TagV('B0:signal', 'internal', path=main_path)

ft.flavorTagger(particleLists = 'B0:signal', weightFiles='B2nunubarBGx1', path=main_path)
#matchMCTruth('pi+:all')
#matchMCTruth(Kres+':all')
#matchMCTruth('gamma:loose')
main_path.add_module('MVAExpert', listNames=['B0:signal'], extraInfoName='CSMVA',
		identifier='./mva-addition/MyTMVA.xml')

ma.writePi0EtaVeto('B0:signal', 'B0 -> pi+:good pi-:good K_S0:all ^gamma', path=main_path)
#myVetoVariables()

kin_variables = vc.mc_truth + ['p', 'E', 'pCMS', 'ECMS', 'cosTheta', 'phi',
        'M', 'dM', 'chiProb','charge', 'PDG']

ft_variables = makePretty(['qrOutput(FBDT)', 'qrOutput(FANN)', 'isRelatedRestOfEventB0Flavor', 'qOutput(FANN)', 'rBinBelle(FANN)',  'qOutput(FBDT)', 'rBinBelle(FBDT)',  'EW90', 'pMissCMS', 'cosThetaMissCMS'])
deltat_variables = ['DeltaT', 'DeltaTErr', 'MCDeltaT', 'TagVz', 'TagVzErr','mcTagVz', 'DeltaZ', 'MCTagBFlavor']
cs_variables = makePretty(['R2', 'cosTBTO', 'cosTBz','CleoConeCS(2)','KSFWVariables(hso02)', 'CleoConeCS(4)', 'CleoConeCS(9)','KSFWVariables(hso10)', 'KSFWVariables(hso12)', 'KSFWVariables(hso14)', 'thrustBm', 'thrustOm'])
roe_vars_clean = makePretty(['ROE_E(cleanMask)','ROE_P(cleanMask)', 'ROE_M(cleanMask)', 'ROE_deltae(cleanMask)', 'ROE_mbc(cleanMask)', 'nROE_Tracks(cleanMask)', 'nROE_NeutralECLClusters(cleanMask)', 'ROE_PTheta(cleanMask)', 'ROE_MC_MissFlags(cleanMask)'])
vtx_variables = ['z', 'z_uncertainty', 'pValue']
dalitz_vars = ['m12', 'm13', 'm23']
mix_variables = []#['mixFBDT', 'mixFANN', 'mixTrue']
cluster_variables = ['clusterReg','clusterTiming',
        'clusterErrorTiming','clusterE1E9',
        'clusterE9E21','clusterAbsZernikeMoment40',
        'clusterAbsZernikeMoment51','minC2TDist',
        'clusterTheta','clusterZernikeMVA',
        'clusterSecondMoment','clusterNHits']

track_variables = vc.track + vc.track_hits + vc.pid 

ma.variablesToNtuple('B0:signal',  
        vc.deltae_mbc+kin_variables+ft_variables+deltat_variables+cs_variables+vtx_variables+mix_variables+roe_vars_clean+dalitz_vars+
        ['CSMVA','isContinuumEvent','isNotContinuumEvent','pi0Likeness','etaLikeness']+
        vu.create_aliases_for_selected(list_of_variables=kin_variables, decay_string=  'B0 -> ^pi+ ^pi- [ ^K_S0 -> ^pi+ ^pi- ] ^gamma')+
        vu.create_aliases_for_selected(list_of_variables=track_variables, decay_string=  'B0 -> ^pi+ ^pi- [ K_S0 -> ^pi+ ^pi- ] gamma')+
        vu.create_aliases_for_selected(list_of_variables=cluster_variables, decay_string=  'B0 -> pi+ pi- [ K_S0 -> pi+ pi- ] ^gamma'),
    	filename = outputFilename, path=main_path)
# Process the events

b2.process(main_path)

# print out the summary
print(b2.statistics)
