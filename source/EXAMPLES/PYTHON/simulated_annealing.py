mc = MainControl.getInstance(0)
ms = MolecularStructure.getInstance(0)
ff = ms.getForceField()
cm = mc.getCompositeManager()
system = mc.getCompositeManager().getComposites()[0]
ff.setup(system)
ssm = SnapShotManager()
#ssm.setSystem(system)
#ssm.setForceField(ff)
mds = CanonicalMD(ff, ssm)
mds.setTimeStep(0.001)
time = 0
starttemperature = 0
zieltemperature = 293
temperature = 0
counter = 0
step = 1.
limit = 100
heatlimit=200
step = float(zieltemperature - starttemperature) / float(limit)
print step
mds.setReferenceTemperature(0)
#pB = PeriodicBoundary(ff)
#pB.setup()
#pB.addSolvent("/home/student/anne/H2O.hin")
temperature = starttemperature
while counter < limit:
 mds.simulateIterations(50)
 mds.setReferenceTemperature(temperature)
 mc.update(system)
 counter = counter + 1
 temperature = float(step)  * float(counter)

temperature = zieltemperature
while counter < heatlimit:
 mds.simulateIterations(50)
 mds.setReferenceTemperature(temperature)
 mc.update(system)
 counter = counter + 1

counter = 0
temperature = zieltemperature
while counter < limit:
 mds.simulateIterations(50)
 mds.setReferenceTemperature(temperature)
 mc.update(system)
 counter = counter - 1
 temperature = float(step) * float(counter)

