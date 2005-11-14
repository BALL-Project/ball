# select all atoms in proximity of less than maxdistance from a selected atom
maxdistance = 4

if getSystems() == []:
	print "No System loaded! aborting..:"

system = getSystem(0)

if getMainControl().getSelection() == []:
	print "No Selection available! Please select some Atoms before starting this script!"

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

