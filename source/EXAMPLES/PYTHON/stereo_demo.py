dp = DisplayProperties.getInstance(0)
dp.selectModel(MODEL_BALL_AND_STICK)
dp.selectColoringMethod(COLORING_ELEMENT)
  
fd = MolecularFileDialog.getInstance(0)
fd.openFile("bpti.pdb")

scene = Scene.getInstance(0)
scene.enterDualStereo()

mc = MainControl.getInstance(0)
ms = MolecularStructure.getInstance(0)
ms.addHydrogens()
ff = ms.getForceField()
cm = mc.getCompositeManager()
c = cm.getComposites()
system = c[0]
ssm = SnapShotManager()
ff.setup(system)
mds = CanonicalMD()
mds.setup(ff, ssm)
mds.setTimeStep(0.001)
counter = 0
limit = 6000000000
pyw = PyWidget.getInstance(0)
while counter < limit and not pyw.toAbortScript():
  mds.simulateIterations(1)
  mc.update(system)

scene.exitStereo()
