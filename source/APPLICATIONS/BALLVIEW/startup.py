mc = MainControl.getInstance(0)
pm = mc.getPrimitiveManager()
cm = mc.getCompositeManager()
dp = DisplayProperties.getInstance(0)
fd = MolecularFileDialog.getInstance(0)
moc = MolecularControl.getInstance(0)

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

def run(file):
	pw = PyWidget.getInstance(0)
	pw.run(file)


