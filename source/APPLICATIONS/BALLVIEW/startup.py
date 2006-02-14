def getMainControl():
	return MainControl.getInstance(0)

def getMolecularStructure():
	return MolecularStructure.getInstance(0)

def getMolecularControl():
	return MolecularControl.getInstance(0)

def getDisplayProperties():
	return DisplayProperties.getInstance(0)

def getComposites():
	return getMainControl().getCompositeManager().getComposites()

def getRepresentations():
	return getMainControl().getPrimitiveManager().getRepresentations()

def clearRepresentations():
	nr = pm.getNumberOfRepresentations()
	rl = pm.getRepresentations()
	i = 0
	while i < nr:
		mc.remove(rl[i])
		i = i + 1

def createStickModel():
	dp.setDrawingPrecision(DRAWING_PRECISION_HIGH)
	dp.selectMode(DRAWING_MODE_SOLID)
	dp.selectModel(MODEL_STICK)
	dp.selectColoringMethod(COLORING_ELEMENT)
	dp.setTransparency(0)
	dp.apply()

def removeWater():
	mc.clearSelection()
	if moc.applySelector("residue(HOH)") == 0:
		return
	moc.cut()

