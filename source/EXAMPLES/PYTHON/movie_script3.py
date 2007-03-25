# create an animation for a molecule

blend_step = 3

clearAll()

s = getScene()
#showOnlyScene()
getDisplayProperties().enableCreationForNewMolecules(0)

setMultithreading(0)
#the molecule to animate
openFile(Path().find("structures/bpti.pdb"))

bsm = AddBallAndStickModel();
r = Representation();
r.setComposites(getSystems())
r.setModelProcessor(bsm);
ecp = ElementColorProcessor();
r.setColorProcessor(ecp);
getMainControl().insert(r);
getMainControl().update(r);

getDisplayProperties().selectModel(MODEL_STICK)
getDisplayProperties().selectColoringMethod(COLORING_ELEMENT)
getDisplayProperties().setDrawingPrecision(DRAWING_PRECISION_HIGH)
getDisplayProperties().createRepresentation(getSystems())

getDisplayProperties().selectModel(MODEL_SE_SURFACE)
getDisplayProperties().selectColoringMethod(COLORING_CUSTOM)
getDisplayProperties().setDrawingPrecision(DRAWING_PRECISION_HIGH)
getDisplayProperties().createRepresentation(getSystems())

# do we want to create a movie with PNGs or POVRays?
def my_task():
	m.processEvents(50000)
	#s.exportPOVRay()
	s.exportPNG()
	
m = getMainControl()
getRepresentations()
r = getRepresentations()[1]
# zoom in on molecule
for i in range(0, 255):
	r.setTransparency(255 - i)
	r.getGeometricObjects()[0].getColor().setAlpha(ColorUnit(i))
	print "Current step: "+str(i)
	i = i + blend_step - 1
	m.update(r)
	my_task()

getDisplayProperties().enableCreationForNewMolecules(1)
setMultithreading(1)
