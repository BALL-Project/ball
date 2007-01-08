#   example script for simulated annealing
#   output heated structures as Hinfile
#   one system has to be loaded before calling the script
if getSystems() == []:
  print "No System loaded! aborting..:"

system = getSystem(0)

ff = getMolecularStructure().getForceField()
ff.setup(system)
ssm = SnapShotManager()
mds = CanonicalMD()
mds.setup(ff, ssm)
mds.setTimeStep(0.001)
mds.setReferenceTemperature(0)
	
time = 0
starttemperature = 0
endtemperature = 600
temperature = 0
counter = 0
step = 1.
limit = 600
heatlimit = 1050
step = float(endtemperature - starttemperature) / float(limit)

temperature = starttemperature
for counter in range(0, limit):
	mds.simulateIterations(50)
	mds.setReferenceTemperature(temperature)
	getMainControl().update(system)
	getMainControl().processEvents(2000)
	temperature = float(step) * float(counter)

temperature = endtemperature
for counter in range(0, heatlimit):
	mds.simulateIterations(1000)
	mds.setReferenceTemperature(temperature)
	getMainControl().update(system)
	getMainControl().processEvents(2000)
	outfile = HINFile("heating1_"+str(counter)+".hin", File.MODE_OUT)
	outfile.write(system)
	outfile.close()

getMainControl().update(system)

temperature = endtemperature
for counter in range(limit, 0):
	mds.simulateIterations(50)
	mds.setReferenceTemperature(temperature)
	getMainControl().update(system)
	getMainControl().processEvents(2000)
	temperature = float(step) * float(counter)

getMainControl().update(system)
print ff.updateEnergy()
