from BALL import Atom, Path, PDBAtom, Vector3, atoms, residues
from VIEW import *

__version__ = '1.5'

"""
	I. Convenience access
"""

""" Ia. Widgets """

def getDatasetControl():
	return DatasetControl.getInstance(0)

def getDisplayProperties():
	return DisplayProperties.getInstance(0)

def getGeometricControl():
	return GeometricControl.getInstance(0)

def getLogView():
	return LogView.getInstance(0)

def getMainControl():
	return MainControl.getInstance(0)

def getMolecularControl():
	return MolecularControl.getInstance(0)

def getMolecularStructure():
	return MolecularStructure.getInstance(0)

def getScene():
	return Scene.getInstance(0)


""" Ib. Second-level getters"""

def getForceField():
	return getMolecularStructure().getForceField()

def getMolecularControlSelection():
	return getMainControl().getMolecularControlSelection()

def getOneSystem():
	"""
		Returns the highlighted system. If none or multiple systems are highlighted,
		this function highlights and returns the first system available.
	"""
	s = getMolecularControlSelection()
	if len(s) != 1:
		print("Warning: Excatly one System should be highlighted! Returning first system available.")
		s.append(getSystems()[0])
	s = s[0].getRoot()
	getMolecularControl().highlight([s])
	return s

def getRepresentations():
	return getMainControl().getRepresentationManager().getRepresentations()

def getSelection():
	return getMainControl().getSelection()

def getSystems():
	return getMainControl().getCompositeManager().getComposites()


"""
	II. Main control
"""

def clearSystems():
	[getMainControl().remove(system) for system in getSystems()]

def clearProject():
	getMainControl().clearData()

def clearRepresentations():
	while len(getRepresentations()) > 0:
		getMainControl().remove(getRepresentations()[0])

def hideAllRepresentations():
	for i in range(len(getRepresentations())):
		getRepresentations()[i].setHidden(1)
		getMainControl().update(getRepresentations()[i])

def loadProject(filename):
	loc = Path().find(filename)
	clearProject()
	return getMainControl().loadBALLViewProjectFile(loc)

def openFile(file):
	return getMainControl().openFile(file)

def quickLoad():
	getMainControl().quickLoad()

def quickSave():
	getMainControl().quickSave()

def quitApplication():
	getMainControl().quit()

def setMultithreading(mode):
	getMainControl().setMultithreading(mode)


"""
	III. Misc
"""

def addOptimizedHydrogens():
	getMolecularControl().highlight([getOneSystem()])
	setMultithreading(False)
	getMolecularStructure().addHydrogens()
	getMolecularControl().applySelector("element(H)")
	setMultithreading(True)
	getMolecularStructure().runMinimization(False)

def highlightLigand():
	removeWater()
	l = [r for r in residues(getOneSystem()) if not r.isAminoAcid()]
	getMolecularControl().highlight(l)
	return l

def printAtomTypesForHighlighted():
	print("Atom types for highlighted Items:")
	for r in getMolecularControlSelection():
		if r.__class__ in (Atom, PDBAtom):
			print(str(r.getFullName(Atom.ADD_RESIDUE_ID)) + " : " + str(r.getType()))
		else:
			for a in atoms(r):
				print(a.getFullName(Atom.ADD_RESIDUE_ID) +" : "+str(a.getType()))

def printAtomTypesForLigands():
	highlightLigand()
	printAtomTypesForHighlighted()

def relaxStructure():
	getMolecularControl().highlight([getOneSystem()])
	getScene().optimizeStructure()

def removeWater():
	getMainControl().clearSelection()
	setMultithreading(False)
	if getMolecularControl().applySelector("residue(HOH)") != 0:
		getMolecularControl().highlightSelection()
		getMolecularControl().cut()
	setMultithreading(True)

def selectByExpression(expression):
	getMainControl().clearSelection()
	getMolecularControl().applySelector(expression)

def selectByExpressionAndSetViewPoint(expression, view_point, look_at, look_up):
	setViewPoint(view_point, look_at, look_up)
	selectByExpression(expression)

def setViewPoint(view_point, look_at, look_up):
	getMainControl().clearSelection()
	camera = getScene().getStage().getCamera()
	camera.setViewPoint(Vector3(*[float(e) for e in view_point.split('|')]))
	camera.setLookAtPosition(Vector3(*[float(e) for e in look_at.split('|')]))
	camera.setLookUpVector(Vector3(*[float(e) for e in look_up.split('|')]))
	getScene().setCamera(camera)
	Scene.getInstance(0).applyStereoDefaults()
	getMainControl().update(getRepresentations()[0])

def showCartoonAndLigand():
	s = getOneSystem()
	clearRepresentations()
	highlightLigand()
	getDisplayProperties().selectModel(MODEL_VDW)
	getDisplayProperties().selectColoringMethod(COLORING_ELEMENT)
	getDisplayProperties().apply()
	getMolecularControl().highlight([s])
	getMolecularControl().centerCamera()
	getDisplayProperties().selectModel(MODEL_CARTOON)
	getDisplayProperties().selectColoringMethod(COLORING_RESIDUE_INDEX)
	getDisplayProperties().apply()

def toggleRepresentationByName(name):
	for rep in getRepresentations():
		if rep.getName() == name:
			rep.setHidden(not rep.isHidden())
			getMainControl().update(rep)

def toggleSelectionByExpression(expression):
	if len(getSelection()) == 0:
		getMolecularControl().applySelector(expression)
	else:
		getMainControl().clearSelection()
