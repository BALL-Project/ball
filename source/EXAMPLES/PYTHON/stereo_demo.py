clearAll()

getDisplayProperties().selectModel(MODEL_STICK)
getDisplayProperties().selectColoringMethod(COLORING_ELEMENT)
setMultithreading(0)  
openFile(Path().find("structures/bpti.pdb"))
getMolecularStructure().addHydrogens()

getScene().enterDualStereo()

system = getSystem(0)
ff = getForceField()
ff.setup(system)
mds = CanonicalMD()
ssm = SnapShotManager()
mds.setup(ff, ssm)
mds.setTimeStep(0.001)
counter = 0
limit = 60
setMultithreading(1)  

while counter < limit and not getPyWidget().toAbortScript():
  mds.simulateIterations(1)
  getMainControl().update(system)
  limit = limit + 1
  getMainControl().processEvents(2000)

getScene().exitStereo()
