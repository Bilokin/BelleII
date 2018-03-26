from basf2 import *
import basf2_mva
from modularAnalysis import *
from variables import variables
variables.addAlias('pi0Likeness','extraInfo(Pi0_Prob)')
variables.addAlias('etaLikeness','extraInfo(Eta_Prob)')

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

variables.addAlias('pi0vetoGamma1E','extraInfo(pi0vetoGamma1E)')
variables.addAlias('pi0vetoGamma1CosTheta','extraInfo(pi0vetoGamma1CosTheta)')
variables.addAlias('pi0vetoGamma1Timing','extraInfo(pi0vetoGamma1Timing)')

PI0ETAVETO_COUNTER = 0

def myVetoVariables(Kres):

	#'''
	# ----------------
	# VETO starts here

	roe_path = create_path()
	deadEndPath = create_path()

	signalSideParticleFilter('B0:signal', '', roe_path, deadEndPath)

	fillParticleList('gamma:roe', 'isInRestOfEvent == 1 and E > 0.050 and cosTheta > -0.65', path=roe_path)

	fillSignalSideParticleList('gamma:sig', 'B0 -> '+Kres+' ^gamma', roe_path)

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

def writePi0EtaVeto(
    particleList,
    decayString,
    workingDirectory='.',
    pi0vetoname='Pi0_Prob',
    etavetoname='Eta_Prob',
    downloadFlag=True,
    selection='',
    path=analysis_main,
):
	"""
	Give pi0/eta probability for hard photon.

	default weight files are set 1.4 GeV as the lower limit of hard photon energy in CMS Frame when mva training for pi0etaveto.
	current default weight files are optimised by MC9.
	The Input Variables are as below. Aliases are set to some variables when training.
	M : pi0/eta candidates Invariant mass
	lowE : soft photon energy in lab frame
	cTheta : soft photon ECL cluster's polar angle
	Zmva : soft photon output of MVA using Zernike moments of the cluster
	minC2Hdist : soft photon distance from eclCluster to nearest point on nearest Helix at the ECL cylindrical radius

	If you don't have weight files in your workingDirectory,
	these files are downloaded from database to your workingDirectory automatically.
	Please refer to analysis/examples/tutorials/B2A306-B02RhoGamma-withPi0EtaVeto.py
	about how to use this function.

	NOTE for debug
	Please don't use following ParticleList names elsewhere.
	'gamma:HARDPHOTON', pi0:PI0VETO, eta:ETAVETO,
	'gamma:PI0SOFT' + str(PI0ETAVETO_COUNTER), 'gamma:ETASOFT' + str(PI0ETAVETO_COUNTER)
	Please don't use "lowE", "cTheta", "Zmva", "minC2Hdist" as alias elsewhere.

	@param particleList     The input ParticleList
	@param decayString specify Particle to be added to the ParticleList
	@param workingDirectory The weight file directory
	@param downloadFlag whether download default weight files or not
	@param pi0vetoname extraInfo name of pi0 probability
	@param etavetoname extraInfo name of eta probability
	@param selection Selection criteria that Particle needs meet in order for for_each ROE path to continue
	@param path       modules are added to this path
	"""
	global PI0ETAVETO_COUNTER

	if PI0ETAVETO_COUNTER == 0:
		variables.addAlias('lowE', 'daughter(1,E)')
		variables.addAlias('cTheta', 'daughter(1,clusterTheta)')
		variables.addAlias('Zmva', 'daughter(1,clusterZernikeMVA)')
		variables.addAlias('minC2Hdist', 'daughter(1,minC2HDist)')

	PI0ETAVETO_COUNTER = PI0ETAVETO_COUNTER + 1

	roe_path = create_path()

	deadEndPath = create_path()

	signalSideParticleFilter(particleList, selection, roe_path, deadEndPath)

	fillSignalSideParticleList('gamma:HARDPHOTON', decayString, path=roe_path)

	pi0softname = 'gamma:PI0SOFT'
	etasoftname = 'gamma:ETASOFT'
	softphoton1 = pi0softname + str(PI0ETAVETO_COUNTER)
	softphoton2 = etasoftname + str(PI0ETAVETO_COUNTER)

	fillParticleList(
		softphoton1,
		'[clusterReg==1 and E>0.025] or [clusterReg==2 and E>0.02] or [clusterReg==3 and E>0.02]',
		path=roe_path)
	applyCuts(softphoton1, 'abs(clusterTiming)<120', path=roe_path)
	fillParticleList(
		softphoton2,
		'[clusterReg==1 and E>0.035] or [clusterReg==2 and E>0.03] or [clusterReg==3 and E>0.03]',
		path=roe_path)
	applyCuts(softphoton2, 'abs(clusterTiming)<120', path=roe_path)

	reconstructDecay('pi0:PI0VETO -> gamma:HARDPHOTON ' + softphoton1, '', path=roe_path)
	reconstructDecay('eta:ETAVETO -> gamma:HARDPHOTON ' + softphoton2, '', path=roe_path)

	if not os.path.isdir(workingDirectory):
		os.mkdir(workingDirectory)
		B2INFO('writePi0EtaVeto: ' + workingDirectory + ' has been created as workingDirectory.')

	if not os.path.isfile(workingDirectory + '/pi0veto.root'):
		if downloadFlag:
			use_central_database('development')
			basf2_mva.download('Pi0VetoIdentifier', workingDirectory + '/pi0veto.root')
			B2INFO('writePi0EtaVeto: pi0veto.root has been downloaded from database to workingDirectory.')

	if not os.path.isfile(workingDirectory + '/etaveto.root'):
		if downloadFlag:
			use_central_database('development')
			basf2_mva.download('EtaVetoIdentifier', workingDirectory + '/etaveto.root')
			B2INFO('writePi0EtaVeto: etaveto.root has been downloaded from database to workingDirectory.')

	roe_path.add_module('MVAExpert', listNames=['pi0:PI0VETO'], extraInfoName='Pi0Veto',
			identifier=workingDirectory + '/pi0veto.root')
	roe_path.add_module('MVAExpert', listNames=['eta:ETAVETO'], extraInfoName='EtaVeto',
			identifier=workingDirectory + '/etaveto.root')

	rankByHighest('pi0:PI0VETO', 'extraInfo(Pi0Veto)', 1, path=roe_path)
	rankByHighest('eta:ETAVETO', 'extraInfo(EtaVeto)', 1, path=roe_path)

	variableToSignalSideExtraInfo('pi0:PI0VETO', {'extraInfo(Pi0Veto)': pi0vetoname}, path=roe_path)
	variableToSignalSideExtraInfo('eta:ETAVETO', {'extraInfo(EtaVeto)': etavetoname}, path=roe_path)
	variableToSignalSideExtraInfo('pi0:PI0VETO', {'daughter(1,E)': 'pi0vetoGamma1E'}, path=roe_path)
	variableToSignalSideExtraInfo('pi0:PI0VETO', {'daughter(1,cosTheta)': 'pi0vetoGamma1CosTheta'}, path=roe_path)
	variableToSignalSideExtraInfo('pi0:PI0VETO', {'daughter(1,clusterTiming)': 'pi0vetoGamma1Timing'}, path=roe_path)

	path.for_each('RestOfEvent', 'RestOfEvents', roe_path)
