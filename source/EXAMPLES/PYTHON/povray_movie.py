#
# Render a simple movie of the current models being rotated around the look-at vector of the camera by
# 360 degrees. Output is written as povray models, so make sure your file system has sufficient space
# left if you render larger surfaces.
#

# The step size in degrees between subsequent frames. 2 obviously leads to 180 frames being generated
step = 5
# directory for storing the POVRay files (Warning: these can get HUGE!)
movie_dir = '.'
# base filename for the movie files. a number and .pob will be appended
movie_base = 'movie'

import os

# Get access to the maincontrol.
mc = MainControl.getInstance(0)
cm = mc.getCompositeManager()
s = Scene.getInstance(0)
camera = Camera(s.getStage().getCamera())
old_viewpoint = Vector3(camera.getViewPoint())

vv = Vector3(camera.getViewPoint())
la = Vector3(camera.getLookAtPosition())

# Compute rotation matrix for rotation about (camera) Y-axis
frame_angle = Angle(step, 0)
matrix = Matrix4x4()
matrix.setRotation(frame_angle, camera.getLookUpVector())

for snap in range(360. / step):
	print "writing snapshot",snap, "of", (360 / step)
	vv = camera.getViewPoint() - la
	vv = Vector3(matrix.m11 * vv.x + matrix.m12 * vv.y + matrix.m13 * vv.z + matrix.m14, matrix.m21 * vv.x + matrix.m22 * vv.y + matrix.m23 * vv.z + matrix.m24, matrix.m31 * vv.x + matrix.m32 * vv.y + matrix.m33 * vv.z + matrix.m34)
	camera.setViewPoint(vv + la)
	s.setCamera(camera)
	filename = os.path.join(movie_dir, movie_base + "_%04d.pov" % snap)
	s.exportScene(POVRenderer(filename))

print "finished"

# Reset the camera.
camera.setViewPoint(old_viewpoint)
s.setCamera(Camera(camera))
