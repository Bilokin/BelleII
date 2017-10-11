# define the mask with hard-coded CS cuts in the ROE mask
# ROEmask tuple: (mask name, track cuts, cluster cuts)
oldMask = ('oldMask', 'useCMSFrame(p)<=3.2', 'p >= 0.05 and useCMSFrame(p)<=3.2')
#Againts beam background
# define the "cleaner" mask
eclCut = '[E > 0.062 and abs(clusterTiming) < 18 and clusterReg==1] or \
[E>0.060 and abs(clusterTiming) < 20 and clusterReg==2] or \
[E>0.056 and abs(clusterTiming) < 44 and clusterReg==3]'
cleanMask = ('cleanMask', 'abs(d0) < 10.0 and abs(z0) < 20.0', eclCut)

# append both masks to ROE
appendROEMasks('B0:phiKs', [oldMask,cleanMask],path=path)

# choose one mask which is applied
buildContinuumSuppression('B0:phiKs', 'cleanMask',path=path)
# define Ntuple tools                                                                                                                                                   
tools = ['EventMetaData', '^B0:phiKs']
tools += ['InvMass', '^B0:phiKs -> [^phi -> K+ K-] ^K_S0']
tools += ['DeltaEMbc', '^B0:phiKs']
tools += ['CustomFloats[isSignal:isContinuumEvent]', '^B0:phiKs']
# add default CS
tools += ['ContinuumSuppression', '^B0:phiKs']

# save stuff to root file                                                                                                                                               
ntupleFile('ana_PhiKs_ntuple_sample.root',path=path)
ntupleTree('b0phiKs', 'B0:phiKs', tools,path=path)
