mc = MainControl.getInstance(0)
cm = mc.getCompositeManager()
c = cm.getComposites()
system = c[0]
for atom1 in atoms(system):
	for atom2 in atoms(system):
		v = atom1.getPosition() - atom2.getPosition()
		if v.getLength() < 1.5:
			atom1.createBond(atom2)

mc.update(system)

