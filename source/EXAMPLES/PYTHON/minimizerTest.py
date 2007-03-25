def printForces():
	atoml = atoms(getOneSystem()[0])
	for i in range(0, len(atoml)):
			print atoml[i].getFullName() + " " + str(atoml[i].getForce().getLength())

def minimize():
	getOneSystem()
	getMolecularStructure().runMinimization(false)
	
map("", "F2", "randomizeAtoms(1.)")
map("", "F3", "minimize()")
map("", "F4", "printForces()")
