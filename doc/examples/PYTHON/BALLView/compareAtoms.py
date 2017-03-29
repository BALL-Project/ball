# example for how to create a new representation
# we draw a line between the identical named atoms of two systems

system1 = getSystems()[0]
system2 = getSystems()[1]

rep = Representation()

for residue1 in residues(system1):
	for residue2 in residues(system2):
		if residue1.getID() == residue2.getID():
			for atom1 in atoms(residue1):
				for atom2 in atoms(residue2):
					if atom1.getName() == atom2.getName():
						line = Line();
						line.setVertex1(atom1.getPosition())
						line.setVertex2(atom2.getPosition())
						line.setColor(ColorRGBA(0,0,1.0))
						rep.insert(line)
						break

getMainControl().insert(rep)
getMainControl().update(rep)

