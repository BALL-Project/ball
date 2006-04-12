# create 2 images with a given distance,
# either as POVRay export file or as a PNG
def stereoImage(dist, povray):
	c = getScene().getStage().getCamera()
	rv = Vector3(c.getRightVector()).normalize()
	vp = Vector3(c.getViewPoint())
	dist = dist / 2.0
	c.setViewPoint(vp - rv * dist)
	setCamera(c)
	if povray == 0:
		getScene().exportPNG()
	else:
		getScene().exportPOVRay()
	c.setViewPoint(vp + rv * dist)
	setCamera(c)
	if povray == 0:
		getScene().exportPNG()
	else:
		getScene().exportPOVRay()
	c.setViewPoint(vp)
	setCamera(c)
	
