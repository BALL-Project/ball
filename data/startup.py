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

# get a list with the Highlighted System or first one
def getOneSystem():
	s = getMolecularControlSelection()
	if len(s) == 0:
		s.append(getSystem(0))
	if len(s) != 1:
		print "One System has to be highlighted!"
		return
	S = s[0] 
	l = []
	l.append(S)
	return l

def getRepresentations():
	return getMainControl().getPrimitiveManager().getRepresentations()

def getForceField():
	return getMolecularStructure().getForceField()

###################### HOTKEYS: #######################
def hideAllRepresentations():
	for i in range(len(getRepresentations())):
		getRepresentations()[i].setHidden(1)
		getMainControl().update(getRepresentations()[i])

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

def quickSave():
	getMainControl().quickSave()

def quickLoad():
	getMainControl().quickLoad()

def removeWater():
	getMainControl().clearSelection()
	setMultithreading(0)
	if getMolecularControl().applySelector("residue(HOH)") == 0:
		return
	getMolecularControl().cut()
	for i in range(len(getSystems())):
		getMainControl().update(getSystem(i), 1)
	setMultithreading(1)

def addOptimizedHydrogens():
	getOneSystem()
	getMolecularStructure().addHydrogens()
	getMolecularControl().applySelector("element(H)")
	getMolecularStructure().runMinimization()

def relaxStructure():	
	s = getOneSystem()
	addOptimizedHydrogens()
	if getMainControl().getSelection() != []:
		print "aborted..."
		return
	getMolecularControl().applySelector("")
	getMolecularControl().highlight(s)
	getMolecularStructure().MDSimulation(0)

def highlightLigand():
	s = getOneSystem()
	removeWater()
	S = s[0] 
	l = []
	for r in residues(S):
		if not r.isAminoAcid():
			l.append(r)
	getMolecularControl().highlight(l)
	return l

def showCartoonAndLigand():
	s = getOneSystem()
	S = s[0] 
	clearRepresentations()
	highlightLigand()
	getDisplayProperties().selectModel(MODEL_VDW)
	getDisplayProperties().selectColoringMethod(COLORING_ELEMENT)
	getDisplayProperties().apply()
	l = []
	l.append(S)
	getMolecularControl().highlight(l)
	getMolecularControl().centerCamera()
	getDisplayProperties().selectModel(MODEL_CARTOON)
	getDisplayProperties().selectColoringMethod(COLORING_RESIDUE_INDEX)
	getDisplayProperties().apply()

def printAtomTypesForHighlighted():
	s = getMolecularControlSelection()
	print "Atom types for highlighted Items:"
	for r in s:
		for a in atoms(r):
			print "Residue "+str(a.getParent().getID())+" "+str(a.getFullName())+" : "+str(a.getType())

def printAtomTypesForLigands():
	highlightLigand()
	printAtomTypesForHighlighted()

###################### EXAMPLES: #######################
def createStickModel():
	dp = getDisplayProperties()
	dp.setDrawingPrecision(DRAWING_PRECISION_HIGH)
	dp.selectMode(DRAWING_MODE_SOLID)
	dp.selectModel(MODEL_STICK)
	dp.selectColoringMethod(COLORING_ELEMENT)
	dp.setTransparency(0)
	dp.apply()


