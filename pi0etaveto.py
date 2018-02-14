from basf2 import *
import basf2_mva
from modularAnalysis import *
from variables import variables
variables.addAlias('pi0Likeness','extraInfo(Pi0_Prob)')
variables.addAlias('etaLikeness','extraInfo(Eta_Prob)')

PI0ETAVETO_COUNTER = 0


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

    path.for_each('RestOfEvent', 'RestOfEvents', roe_path)
