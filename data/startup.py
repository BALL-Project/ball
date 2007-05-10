from sip import *
from random import *
###################### defines: ############################
true=1
false=0

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

def getEScene():
	return EditableScene.getInstance(0)

def getGeometricControl():
	return GeometricControl.getInstance(0)

def getLogView():
	return LogView.getInstance(0)

def getPyWidget():
	return PyWidget.getInstance(0)

def getDatasetControl():
	return DatasetControl.getInstance(0)

def hideAllWidgets():
	getScene().setWidgetVisible(0)
	getGeometricControl().setWidgetVisible(0)
	getMolecularControl().setWidgetVisible(0)
	getLogView().setWidgetVisible(0)
	getDatasetControl().setWidgetVisible(0)
	getPyWidget().setWidgetVisible(0)

def showOnlyScene():
	hideAllWidgets()
	getScene().setWidgetVisible(1)
	getMainControl().processEvents(5000)

def setSceneSize(width, height):
	getMainControl().setContentSize(width, height)
	showOnlyScene()
	getScene().update(false)

###################### SHORTCUTS: #######################

# map a key to a python command:
# modifier can be "" or None, Shift, Ctrl
def map(modifier, key, command):
	getPyWidget().map(modifier, key, command)

def log(to_log):
	getMainControl().setStatusbarText(to_log, 1)
	print to_log

def abortScript():
	getPyWidget().abortScript()

def quit():
	getMainControl().quit()

def run(file):
	getPyWidget().runFile(file)

def openFile(file):
	return getMainControl().openFile(file)
	
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
	if len(s) == 0 or len(s) > 1:
		print "Warning: One System should be highlighted!"
		s.append(getSystem(0))
	S = s[0].getRoot()
	l = []
	l.append(S)
	getMolecularControl().highlight(l)
	return l

def getRepresentations():
	return getMainControl().getRepresentationManager().getRepresentations()

def getForceField():
	return getMolecularStructure().getForceField()

def setCamera(camera):
	getScene().setCamera(camera)

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
	getPyWidget().runFile(filename)

def runCurrentScript():
	getPyWidget().runCurrentScript()

def quickSave():
	getMainControl().quickSave()

def quickLoad():
	getMainControl().quickLoad()

def removeWater():
	getMainControl().clearSelection()
	setMultithreading(0)
	if getMolecularControl().applySelector("residue(HOH)") == 0:
		setMultithreading(1)
		return
	getMolecularControl().highlightSelection()
	getMolecularControl().cut()
	setMultithreading(1)

def addOptimizedHydrogens():
	getOneSystem()
	setMultithreading(0)
	getMolecularStructure().addHydrogens()
	getMolecularControl().applySelector("element(H)")
	setMultithreading(1)
	getMolecularStructure().runMinimization(false)

def relaxStructure():	
	s = getOneSystem()
	getEScene().optimizeStructure()

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

def randomizeAtoms(md):
	atoml = atoms(getOneSystem()[0])
	for i in range(0, len(atoml)):
		atoml[i].setPosition(atoml[i].getPosition() + Vector3(uniform(-md,md), uniform(-md,md), uniform(-md,md)))
	getMainControl().update(getOneSystem()[0])

###################### EXAMPLES: #######################
def createStickModel():
	dp = getDisplayProperties()
	dp.setDrawingPrecision(DRAWING_PRECISION_HIGH)
	dp.selectMode(DRAWING_MODE_SOLID)
	dp.selectModel(MODEL_STICK)
	dp.selectColoringMethod(COLORING_ELEMENT)
	dp.setTransparency(0)
	dp.apply()

def createChainSurfaces():
	clearRepresentations()
	s = getOneSystem()[0]
	getDisplayProperties().selectModel(MODEL_SE_SURFACE)
	getDisplayProperties().selectColoringMethod(COLORING_CHAIN)
	for c in chains(s):
		l = []
		l.append(c)
		getDisplayProperties().createRepresentation(l)


