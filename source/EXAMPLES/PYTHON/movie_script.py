angle_step = 6
turns = 0.5
move_step = 1
move_dist = 100

#s.exportPOVRay()
def my_task():
	s.exportPNG()
	
s = getScene()
m = getMainControl()
camera = Camera(s.getStage().getCamera())

# move to back
s.move(Vector3(0,0,-move_dist))
my_task()

# zoom in on molecule
i = 0
while i < move_dist:
	s.move(Vector3(0,0,move_step * 3))
	m.processEvents(1000)
	i = i + move_step * 3
	my_task()
	
#rotate left
i = 0
while i < 360 * turns:
	s.rotate(angle_step, 0)
	i = i + angle_step
	m.processEvents(1000)
	my_task()
	
#rotate up
i = 0
while i < 360 * turns:
	s.rotate(0, angle_step)
	i = i + angle_step
	m.processEvents(1000)
	my_task()

#rotate left 90 degree
i = 0
while i < 90:
	s.rotate(angle_step, 0)
	i = i + angle_step
	m.processEvents(1000)
	my_task()
	
#move throught molecule
i = 0
while i < move_dist * 3:
	s.move(Vector3(0,0,move_step))
	m.processEvents(1000)
	i = i + move_step
	my_task()

s.setCamera(camera)
