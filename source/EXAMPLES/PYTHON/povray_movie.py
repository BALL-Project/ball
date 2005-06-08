#
# Render a simple movie of the current models being rotated around the look-at vector of the camera by
# 360 degrees. Output is written as povray models, so make sure your file system has sufficient space
# left if you render larger surfaces.
#

# The step size in degrees between subsequent frames. 2 obviously leads to 180 frames being generated
step = 2

# Get access to the maincontrol.
mc = MainControl.getInstance(0)
cm = mc.getCompositeManager()
s = Scene.getInstance(0)
camera = s.getStage().getCamera()


matrix = Matrix4x4()
frame_angle = Angle(step, 0)
matrix.setRotation(frame_angle, camera.getLookUpVector())
vv = camera.getViewVector()
vv.normalize()
vv *= 10.0
la = camera.getLookAtPosition()


for snap in range(360/step):
	print snap, step * snap
	vv = Vector3 (matrix.m11 * vv.x + matrix.m12 * vv.y + matrix.m13 * vv.z + matrix.m14, matrix.m21 * vv.x + matrix.m22 * vv.y + matrix.m23 * vv.z + matrix.m24, matrix.m31 * vv.x + matrix.m32 * vv.y + matrix.m33 * vv.z + matrix.m34);
	camera.setViewPoint(la - vv)
	s.getStage().moveCameraTo(camera)
	s.setCamera(camera)
	s.exportScene(POVRenderer("movie_%04d.pov" % snap))

print "finished"
