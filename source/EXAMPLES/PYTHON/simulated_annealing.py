#   example script for simulated annealing
#   output heated structures as Hinfile
#   one system has to be loaded before calling the script
system = getComposites()[0]
ff = getMolecularStructure().getForceField()
ff.setup(system)
ssm = SnapShotManager()
mds = CanonicalMD()
mds.setup(ff, ssm)
mds.setTimeStep(0.001)
mds.setReferenceTemperature(0)
	
time = 0
starttemperature = 0
endtemperature =600
temperature = 0
counter = 0
step = 1.
limit = 600
heatlimit=1050
step = float(endtemperature - starttemperature) / float(limit)

temperature = starttemperature
while counter < limit:
  mds.simulateIterations(50)
  mds.setReferenceTemperature(temperature)
  getMainControl().update(system)
  counter = counter + 1
  temperature = float(step)  * float(counter)

counter = 0
temperature = endtemperature
while counter < heatlimit:
  mds.simulateIterations(1000)
  mds.setReferenceTemperature(temperature)
  getMainControl().update(system)
  outfile = HINFile("heating1_"+str(counter)+".hin", File.OUT)
  outfile.write(system)
  outfile.close()
  counter = counter + 1

getMainControl().update(system)

counter = limit
temperature = endtemperature
while counter > 0:
 mds.simulateIterations(50)
 mds.setReferenceTemperature(temperature)
 getMainControl().update(system)
 counter = counter - 1
 temperature = float(step) * float(counter)

getMainControl().update(system)
print ff.updateEnergy()
