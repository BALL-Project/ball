#  example script to viualise the movement in a trajectory
dcd = DCDFile("test.dcd")
system = getComposites()[0]
number_ss = dcd.getNumberOfSnapShots()

ssm = SnapShotManager(system, getMolecularStructure().getForceField(), dcd)

rep = Representation()

positions = []
for atom in atoms(system):
	positions.append(atom.getPosition())


while ssm.applyNextSnapShot():
	i = 0
	for atom in atoms(system):
		line = Line()
		line.setVertex1(positions[i])
		print atom.getPosition()
		print positions[i]
		positions[i] = Vector3() + atom.getPosition()
		line.setVertex2(atom.getPosition())
		line.setComposite(atom)
		line.setColor(ColorRGBA(0,0,1.0))
		rep.insert(line)
		i = i + 1

getMainControl().insert(rep)
getMainControl().update(rep)
getMainControl().update(system)

