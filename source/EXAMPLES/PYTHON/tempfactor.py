# color a representation transparent in relation to the temp factor
max_value = 9
rep = getRepresentations()[0]
# enable transparency for the representation
rep.setTransparency(190)
# disable model and coloring recalculation
rep.enableColoringUpdate(false)
rep.enableModelUpdate(false)
gos = rep.getGeometricObjects()
for i in range(len(gos)):
	go = gos[i]
	atoml = all_atoms(go.getComposite())
	if len(atoml) == 0:
		continue
	tf  = 0
	for j in range(len(atoml)):
		tf = tf + cast(atoml[j],PDBAtom).getTemperatureFactor()
	tf = tf * 255/ (len(atoml) * max_value)
	go.getColor().setAlpha(ColorUnit(int(tf)))
	
getMainControl().update(rep)
