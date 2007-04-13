#  example script to viualise the movement in a trajectory
clearAll()
dp = getDisplayProperties()
dp.selectModel(MODEL_LINES)
dp.selectColoringMethod(COLORING_ELEMENT)
dp.enableCreationForNewMolecules(1)
openFile(Path().find("structures/AlaAla.hin"))
dcd = DCDFile(Path().find("structures/alaala.dcd"))
system = getSystem(0)
ssm = SnapShotManager(system, AmberFF(), dcd)
rep = Representation()
current_ss = 0

while ssm.applyNextSnapShot():
	current_ss +=  1
	ratio = float(current_ss) / float(dcd.getNumberOfSnapShots())
	for atom in atoms(system, "!element(H)"):
		sphere = Sphere()
		sphere.setPosition(atom.getPosition())
		sphere.setComposite(atom)
		sphere.setColor(ColorRGBA(1.0 - ratio, 1.0 - ratio, ratio))
		sphere.setRadius(0.03)
		rep.insert(sphere)			

getMainControl().insert(rep)
getMainControl().update(rep)
getMainControl().update(system)

