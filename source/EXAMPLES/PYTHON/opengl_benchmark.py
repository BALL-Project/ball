fd = MolecularFileDialog.getInstance(0)
pm = getMainControl().getPrimitiveManager()
pm.setMultithreadingMode(0)

result = 0
nr_runs = 70

def runTest(name, model):
	global result
	run = 0
	getDisplayProperties().selectModel(model)
	getDisplayProperties().apply()
	v = Vector3(13, 84, 41)
	l = Vector3(13, 84, -7)
	u = Vector3(0, -1, 0)
	s = getScene().getStage()
	s.getCamera().setViewPoint(v)
	s.getCamera().setLookAtPosition(l)
	s.getCamera().setLookUpVector(v)
	sm = SceneMessage(SceneMessage.UPDATE_CAMERA)
	sm.setStage(s)
	getMainControl().sendMessage(sm)
	timer = Timer()
	timer.start()
	while run < nr_runs:
		run += 1
		v = v + Vector3(0,0,1)
		l = l + Vector3(0,0,1)
		s.getCamera().setViewPoint(v)
		sm = SceneMessage(SceneMessage.UPDATE_CAMERA)
		sm.setStage(s)
		getMainControl().sendMessage(sm)
	timer.stop()
	model_result = timer.getClockTime()
	clearRepresentations()
	print name+" "+str(model_result)+" seconds"
	result += model_result

#clearRepresentations()
pm.setMultithreadingMode(0)
dp = getDisplayProperties()
dp.enableCreationForNewMolecules(0)
dp.setDrawingPrecision(DRAWING_PRECISION_HIGH)
dp.setSurfaceDrawingPrecision(6.5)
dp.selectMode(DRAWING_MODE_SOLID)
dp.selectColoringMethod(COLORING_ELEMENT)
dp.setTransparency(0)
fd.openFile(Path().find("structures/bpti.pdb"))

getScene().setVisible(1)
getGeometricControl().getInstance(0).setVisible(0)
getMolecularControl().getInstance(0).setVisible(0)
getLogView().setVisible(0)
getDatasetControl().setVisible(0)
PyWidget.getInstance(0).setVisible(0)

getMainControl().resize(800, 600)
getMainControl().processEvents(5000)

runTest("Lines", 		MODEL_LINES)
runTest("VDW",   		MODEL_VDW)
runTest("BAS",   		MODEL_BALL_AND_STICK)
runTest("Cartoon", 	MODEL_CARTOON)
runTest("SES", 			MODEL_SE_SURFACE)

print "Result: "+str(result)+" seconds"
result /= nr_runs
result = 1 / result
#scale results to the graphics_benchmark.py script
result /= 5
print "Result: "+str(result)+" BALLView OpenGL stones"
pm.setMultithreadingMode(1)
