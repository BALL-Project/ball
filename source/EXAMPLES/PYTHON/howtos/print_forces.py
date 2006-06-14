atoml = atoms(getSystem(0))
for i in range(0, len(atoml)):
	print atoml[i].getFullName() + " " + str(atoml[i].getForce().getLength())

