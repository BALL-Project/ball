###################### MAIN WIDGETS: #######################
def getMainControl():
	return MainControl.getInstance(0)

def getMolecularStructure():
	return MolecularStructure.getInstance(0)

def getMolecularControl():
	return MolecularControl.getInstance(0)

def getDisplayProperties():
	return DisplayProperties.getInstance(0)

def getScene():
	return Scene.getInstance(0)

def getGeometricControl():
	return GeometricControl.getInstance(0)

def getLogView():
	return LogView.getInstance(0)

def getPyWidget():
	return PyWidget.getInstance(0)

def getDatasetControl():
	return DatasetControl.getInstance(0)

def hideAllWidgets():
	getScene().setVisible(0)
	getGeometricControl().setVisible(0)
	getMolecularControl().setVisible(0)
	getLogView().setVisible(0)
	getDatasetControl().setVisible(0)
	getPyWidget().setVisible(0)

def showOnlyScene():
	hideAllWidgets()
	getScene().setVisible(1)
	getMainControl().resize(800, 654)
	getMainControl().processEvents(5000)

###################### SHORTCUTS: #######################
def openFile(file):
	return MolecularFileDialog.getInstance(0).openFile(file)
	
def getSystems():
	return getMainControl().getCompositeManager().getComposites()

def getSystem(nr):
	return getSystems()[nr]

def getSelection():
	return getMainControl().getSelection()

def getMolecularControlSelection():
	return getMainControl().getMolecularControlSelection()

def getRepresentations():
	return getMainControl().getPrimitiveManager().getRepresentations()

def getForceField():
	return getMolecularStructure().getForceField()

def clearRepresentations():
	while len(getRepresentations()) > 0:
		getMainControl().remove(getRepresentations()[0])

def clearMolecules():
	while len(getSystems()) > 0:
		getMainControl().remove(getSystem(0))

def clearAll():
	clearMolecules()
	clearRepresentations()

def setMultithreading(mode):
	getMainControl().setMultithreading(mode)

def runScript(filename):
	getPyWidget().run(filename)

def runScriptAgain():
	getPyWidget().runAgain()
	

###################### EXAMPLES: #######################
def createStickModel():
	dp = getDisplayProperties()
	dp.setDrawingPrecision(DRAWING_PRECISION_HIGH)
	dp.selectMode(DRAWING_MODE_SOLID)
	dp.selectModel(MODEL_STICK)
	dp.selectColoringMethod(COLORING_ELEMENT)
	dp.setTransparency(0)
	dp.apply()

def removeWater():
	getMainControl().clearSelection()
	if getMolecularControl().applySelector("residue(HOH)") == 0:
		return
	getMolecularControl().cut()


