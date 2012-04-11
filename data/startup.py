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

#def getEScene():
#	return EditableScene.getInstance(0)

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
	getPyWidget().openFile(file, true)

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
		log("Warning: One System should be highlighted!")
		s.append(getSystem(0))
	S = s[0].getRoot()
	l = []
	l.append(S)
	getMolecularControl().highlight(l)
	return l

def getRepresentations():
	return getMainControl().getRepresentationManager().getRepresentations()

def getRepresentationByName(name):
  """Return the first representation with the given name"""
  reps = getRepresentations()
  for rep in reps:
    if rep.getName() == name:
      return rep
  return None

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
	getPyWidget().openFile(filename, true)

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
	getScene().optimizeStructure()

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
	log("Atom types for highlighted Items:")
	for r in s:
		if r.__class__ == BALL.Atom:
			log(str(r.getFullName(Atom.ADD_RESIDUE_ID))+" : "+str(r.getType()))
		else:
			for a in atoms(r):
				log(a.getFullName(Atom.ADD_RESIDUE_ID) +" : "+str(a.getType()))

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
	dp.selectModel(MODEL_VDW)
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

def addPlane(plane_specifier, height, boundary, bottom = True):
  systems = getSystems()
  system = systems[0]
  bbp = BoundingBoxProcessor()
  system.apply(bbp)
  v_low = Vector3(bbp.getLower().x, bbp.getLower().y, bbp.getLower().z)
  v_upp = Vector3(bbp.getUpper().x, bbp.getUpper().y, bbp.getUpper().z)
  #
  if (len(systems) > 1):
    for system in systems[1:]:
      system.apply(bbp)
      low = bbp.getLower()
      upp = bbp.getUpper()
      #
      if v_low.x > low.x:
        v_low.x = low.x
      if v_low.y > low.y:
        v_low.y = low.y
      if v_low.z > low.z:
        v_low.z = low.z
      #
      if v_upp.x < upp.x:
        v_upp.x = upp.x 
      if v_upp.y < upp.y:
        v_upp.y = upp.y
      if v_upp.z < upp.z:
        v_upp.z = upp.z
  #
  if (not bottom):
    v_tmp = v_low
    v_low = v_upp
    v_upp = v_tmp
    height = height*(-1)
    boundary = boundary*(-1)
  #
  v_low_left = Vector3()
  v_low_right = Vector3()
  v_upp_right = Vector3()
  v_upp_left = Vector3()
  #
  normal = Vector3()
  #
  if (plane_specifier == 'x'):
    v_low = v_low - Vector3(height, boundary, boundary)
    v_upp = v_upp + Vector3(height, boundary, boundary)
    #
    v_low_left  = Vector3(v_low.x, v_low.y, v_low.z)
    v_low_right = Vector3(v_low.x, v_upp.y, v_low.z)
    v_upp_right = Vector3(v_low.x, v_upp.y, v_upp.z)
    v_upp_left  = Vector3(v_low.x, v_low.y, v_upp.z)
    #
    normal = Vector3(1, 0, 0)
  #
  elif (plane_specifier == 'y'):
    v_low = v_low - Vector3(boundary, height, boundary)
    v_upp = v_upp + Vector3(boundary, height, boundary)
    #
    v_low_left  = Vector3(v_low.x, v_low.y, v_low.z)
    v_low_right = Vector3(v_low.x, v_low.y, v_upp.z)
    v_upp_right = Vector3(v_upp.x, v_low.y, v_upp.z)
    v_upp_left  = Vector3(v_upp.x, v_low.y, v_low.z)
    #
    normal = Vector3(0, 1, 0)
    #
  elif (plane_specifier == 'z'):
    v_low = v_low - Vector3(boundary, boundary, height)
    v_upp = v_upp + Vector3(boundary, boundary, height)
    #
    v_low_left  = Vector3(v_low.x, v_low.y, v_low.z)
    v_low_right = Vector3(v_low.x, v_upp.y, v_low.z)
    v_upp_right = Vector3(v_upp.x, v_upp.y, v_low.z)
    v_upp_left  = Vector3(v_upp.x, v_low.y, v_low.z)
    #
    normal = Vector3(0, 0, 1)
    #
  plane = Mesh()
  #
  # the vertices
  plane.pushBackVertex(v_low_left)
  plane.pushBackVertex(v_low_right)
  plane.pushBackVertex(v_upp_right)
  plane.pushBackVertex(v_upp_left)
  #
  # the triangles
  t1 = Mesh.Triangle()
  t1.v1 = 0 # v_low_left
  t1.v2 = 1 # v_low_right
  t1.v3 = 2 # v_upp_right
  #
  plane.pushBackTriangle(t1)
  #
  t2 = Mesh.Triangle()
  t2.v1 = 2 # v_upp_right
  t2.v2 = 3 # v_upp_left
  t2.v3 = 0 # v_low_left
  #
  plane.pushBackTriangle(t2)
  #
  # the normals
  for i in range(4):
    plane.pushBackNormal(normal)
  #
  color = ColorRGBA(0.2,0.2,0.2,1)
  #
  colors = plane.getColors()
  colors.append(color)
  plane.setColors(colors)
  #
  r = Representation()
  r.insert(plane)
  r.setModelType(MODEL_PLANE)
  #
  getMainControl().insert(r)
  getMainControl().update(r)

import os

if os.environ.has_key('BALLVIEW_ENTER_STEREO') and (os.environ['BALLVIEW_ENTER_STEREO'].upper() == "TRUE"):
  Scene.getInstance(0).enterStereo()

if os.environ.has_key('BALLVIEW_FULLSCREEN') and (os.environ['BALLVIEW_FULLSCREEN'].upper() == "TRUE"):
  getMainControl().showFullScreen()

