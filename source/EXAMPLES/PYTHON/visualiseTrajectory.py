#  example script to viualise the movement in a trajectory
clearAll()
getDisplayProperties().selectModel(MODEL_LINES)
getDisplayProperties().selectColoringMethod(COLORING_ELEMENT)
getDisplayProperties().enableCreationForNewMolecules(1)
openFile(Path().find("structures/AlaAla.hin"))
dcd = DCDFile(Path().find("structures/alaala.dcd"))
system = getSystem(0)
number_ss = dcd.getNumberOfSnapShots()

ssm = SnapShotManager(system, getMolecularStructure().getForceField(), dcd)
nr_ss = dcd.getNumberOfSnapShots()

rep = Representation()
rep.setDrawingPrecision(DRAWING_PRECISION_MEDIUM)

positions = []
for atom in atoms(system):
	if atom.getElement().getSymbol() == "H":
		continue
	positions.append(atom.getPosition())

	
current_ss = 0
while ssm.applyNextSnapShot():
	current_ss = current_ss + 1
	ratio = float(current_ss) / float(nr_ss)
	i = 0
	for atom in atoms(system):
		if atom.getElement().getSymbol() == "H":
			continue
		sphere = Sphere()
		sphere.setPosition((positions[i] + atom.getPosition()) / 2.0)
		sphere.setComposite(atom)
		sphere.setColor(ColorRGBA(1.0 - ratio, 1.0 - ratio, ratio))
		sphere.setRadius(0.03)
		rep.insert(sphere)			
		i = i + 1

getMainControl().insert(rep)
getMainControl().update(rep)
getMainControl().update(system)

