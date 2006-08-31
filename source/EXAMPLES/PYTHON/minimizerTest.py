from random import *
	
def printForces():
	atoml = atoms(getSystem(0))
	for i in range(0, len(atoml)):
			print atoml[i].getFullName() + " " + str(atoml[i].getForce().getLength())

def randomize():
	atoml = atoms(getSystem(0))
	for i in range(0, len(atoml)):
		atoml[i].setPosition(atoml[i].getPosition() + Vector3(uniform(-1,1), uniform(-1,1), uniform(-1,1)))
	getMainControl().update(getSystem(0))

map("", "F2", "randomize()")
map("", "F3", "getMolecularStructure().runMinimization(false)")
map("", "F4", "printForces()")
