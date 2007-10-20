atoml = atoms(getSystem(0))
for i in range(0, len(atoml)):
	print atoml[i].getFullName() + " " + str(atoml[i].getForce().x) + " " + str(atoml[i].getForce().y) + " " + str(atoml[i].getForce().z
	)

