atoms = atoms(getSystem(0))
for i in range(0, len(atoms)):
	print atoms[i].getFullName() + " " + str(atoms[i].getForce().getLength())

