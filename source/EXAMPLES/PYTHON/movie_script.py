# create an animation for a molecule

clearAll()

#the molecule to animate
openFile(Path().find("structures/bpti.pdb"))

angle_step = 6
turns = 1
move_step = 1
move_dist = 100

# do we want to create a movie with PNGs or POVRays?
def my_task():
	m.processEvents(1000)
	#s.exportPOVRay()
	#s.exportPNG()
	
s = getScene()
m = getMainControl()
camera = Camera(s.getStage().getCamera())

# zoom out
s.move(Vector3(0,0,-move_dist))

# zoom in on molecule
for i in range(0, move_dist / move_step):
	s.move(Vector3(0,0,move_step))
	my_task()
	
#rotate left
for i in range(0, 360 * turns / angle_step):
	s.rotate(angle_step, 0)
	my_task()
	
#rotate up
for i in range(0, 360 * turns / angle_step):
	s.rotate(0, angle_step)
	my_task()

#rotate left 90 degree
for i in range(0, 90 / angle_step):
	s.rotate(angle_step, 0)
	my_task()
	
#move throught molecule
for i in range(0, move_dist * 2 / move_step):
	s.move(Vector3(0,0,-move_step))
	my_task()

#reset camera
s.setCamera(camera)
