mc = MainControl.getInstance(0)
ms = MolecularStructure.getInstance(0)
ff = ms.getForceField()
cm = mc.getCompositeManager()
c = cm.getComposites()
system = c[0]
mds = CanonicalMD()
ssm = SnapShotManager()
ff.setup(system)
mds.setup(ff, ssm)
mds.setTimeStep(0.001)
time = 0
temperature = 0
mds.setReferenceTemperature(0)
while time < 100:
 mds.simulateIterations(50)
 mds.setReferenceTemperature(temperature)
 mc.update(system)
 time = time + 1
 temperature = 293. / 100. * float(time)


