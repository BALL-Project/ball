#  example script to viualise how mobile all atoms from a trajectory are
#  it creates a sphere for every atoms
#  the radius is proportional to the mobility and the color is
#  interpolated between blue (no movement at all) and red (very mobile)
dcd = DCDFile("test.dcd")
system = getSystems()[0]
number_ss = dcd.getNumberOfSnapShots()

ssm = SnapShotManager(system, getMolecularStructure().getForceField(), dcd)

rep = Representation()

positions = []
distances = []

nr_atoms = 0
for atom in atoms(system):
	positions.append(Vector3())
	distances.append(0)
	nr_atoms += 1


# calculate the median position of every atom
while ssm.applyNextSnapShot():
	i = 0
	for atom in atoms(system):
		positions[i] = positions[i] + atom.getPosition()
		i = i + 1

for position in positions:
  position /= number_ss

ssm.applyFirstSnapShot()

# calculate the medium difference in position for every atom
while ssm.applyNextSnapShot():
	i = 0
	for atom in atoms(system):
		distances[i] = distances[i] + (atom.getPosition() - positions[i]).getLength()
		i = i + 1

#normalize distances
max_distance = 0
for distance in distances:
	if distance > max_distance:
		max_distance = distance

i = 0
while i < nr_atoms:
	distances[i] = distances[i] / max_distance
	i = i + 1
 
# create the spheres
i = 0
while i < nr_atoms:
	sphere = Sphere()
	sphere.setPosition(positions[i])
	sphere.setRadius(distances[i])
	sphere.setColor(ColorRGBA(distances[i], 0, 1 -distances[i]))
	rep.insert(sphere)
	i = i + 1

# set the atoms to their median position
i = 0
for atom in atoms(system):
	atom.setPosition(positions[i])
	i = i + 1

getMainControl().insert(rep)
getMainControl().update(rep)
getMainControl().update(system)

