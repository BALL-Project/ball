# create an animation for a molecule

# do we want to create a movie with PNGs or POVRays?
def my_task():
	m.processEvents(1000)
	#s.exportPOVRay()
	s.exportPNG()
	
s = getScene()
mc = getMolecularControl()
m = getMainControl()

# delete old representations and disable new representations 
clearAll()
getDisplayProperties().enableCreationForNewMolecules(0)

#the molecule to animate
openFile(Path().find("structures/1a1f.pdb"))
system = getSystem(0)

# obtain the different chains
chain_a = []
chain_a.append(chains(system)[0])

chain_b_c = []
chain_b_c.append(chains(system)[1])
chain_b_c.append(chains(system)[2])

# select model to create later
getDisplayProperties().selectModel(MODEL_CARTOON)
getDisplayProperties().selectColoringMethod(COLORING_CHAIN)

# create the representation
mc.highlight(chain_a)
m.processEvents(1000)
# we have to wait for the new representations
setMultithreading(0)
getDisplayProperties().apply()

getDisplayProperties().selectColoringMethod(COLORING_RESIDUE_INDEX)
mc.highlight(chain_b_c)
m.processEvents(1000)
getDisplayProperties().apply()

# we are done with the representations:
getDisplayProperties().enableCreationForNewMolecules(1)
setMultithreading(1)

#set camera point
camera = Camera(s.getStage().getCamera())
camera.setViewPoint(Vector3(20.2,-24.9,19.3))
camera.setLookAtPosition(Vector3(3.0,11.1,47.3))
camera.setLookUpVector(Vector3(0.9,0.47,0))
s.setCamera(camera)

#rotate right
angle_step = 2
for i in range(0, 390 / angle_step):
	s.rotate(angle_step, 0)
	my_task()
	
# move chain C
move_step = 1
for i in range(0, 45 / move_step): 
	s.moveComposites(chain_a, Vector3(-move_step, move_step, 0))
	my_task()

#remove chain a
setMultithreading(0)
mc.highlight(chain_a)
mc.cut()
setMultithreading(1)
	
#rotate right
for i in range(0, 360 / angle_step):
	s.rotate(angle_step, 0)
	my_task()

