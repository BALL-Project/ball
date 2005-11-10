result = 0
nr_runs = 70

def runTest(name, model):
	global result
	run = 0
	getDisplayProperties().selectModel(model)
	getDisplayProperties().apply()
	getGeometricControl().focusRepresentation()
	getScene().move(Vector3(0,0,-nr_runs))
	timer = Timer()
	timer.start()
	while run < nr_runs:
		run += 1
		getScene().move(Vector3(0,0,1))
	timer.stop()
	model_result = timer.getClockTime()
	clearRepresentations()
	print name+" "+str(model_result)+" seconds"
	result += model_result

setMultithreading(0)
clearMolecules()
clearRepresentations()
dp = getDisplayProperties()
dp.enableCreationForNewMolecules(0)
dp.setDrawingPrecision(DRAWING_PRECISION_HIGH)
dp.setSurfaceDrawingPrecision(6.5)
dp.selectMode(DRAWING_MODE_SOLID)
dp.selectColoringMethod(COLORING_ELEMENT)
dp.setTransparency(0)
openFile(Path().find("structures/bpti.pdb"))

getScene().setVisible(1)
getGeometricControl().getInstance(0).setVisible(0)
getMolecularControl().getInstance(0).setVisible(0)
getLogView().setVisible(0)
getDatasetControl().setVisible(0)
getPyWidget().setVisible(0)

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
setMultithreading(1)
