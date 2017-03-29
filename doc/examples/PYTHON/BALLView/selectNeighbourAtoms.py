# select all atoms in proximity of less than maxdistance from a selected atom
maxdistance = 4

system = getSystems()[0]

cl = []

for atom1 in atoms(system):
	if not atom1.isSelected(): 
		continue
	for atom2 in atoms(system):
		dist = (atom1.getPosition() - atom2.getPosition()).getLength()
		if not atom2.isSelected() and dist < maxdistance:
			cl.append(atom2)

system.deselect()

for atom in cl:
	atom.select()

getMainControl().update(system)

