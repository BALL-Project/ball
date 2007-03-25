# create a movie from a previously build grid slice
# parameters to adapt:
steps = 20
step = 0.2

# do we want to create a movie with PNGs or POVRays?
def my_task():
  getMainControl().processEvents(50000)
  #getScene().exportPOVRay()
  getScene().exportPNG()

reps = getRepresentations()
found = 0
for rep in reps:
	if rep.getModelType() == MODEL_GRID_SLICE:
		found = true
		break

if found == 0:
	log("No grid slice available!")
	abortScript()

slice = rep.getGeometricObjects()[0]
print slice
point = slice.getPoint()
normal = slice.getNormal()
new_point = point

s = 0
while s <= steps:
	my_task()
	new_point += normal * step
	slice.setPoint(new_point)
	getMainControl().update(rep)
	s += 1

slice.setPoint(point)
getMainControl().update(rep)
