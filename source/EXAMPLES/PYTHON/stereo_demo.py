getDisplayProperties().selectModel(MODEL_BALL_AND_STICK)
getDisplayProperties().selectColoringMethod(COLORING_ELEMENT)
  
openFile("bpti.pdb")
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

while counter < limit and not getPyWidget().toAbortScript():
  mds.simulateIterations(10)
  getMainControl().update(system)
  limit = limit + 10

getScene().exitStereo()
