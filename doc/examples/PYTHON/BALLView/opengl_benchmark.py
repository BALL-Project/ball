result = 0
nr_runs = 70

def runTest(name, model):
	global result
	getDisplayProperties().selectModel(model)
	getDisplayProperties().createRepresentation(getSystems())
	getGeometricControl().focus()
	getScene().move(Vector3(0,0,-nr_runs))
	timer = Timer()
	timer.start()
	for run in range(0, nr_runs):
		getScene().move(Vector3(0,0,1))
	timer.stop()
	model_result = timer.getClockTime()
	clearRepresentations()
	print name+" "+str(model_result)+" seconds"
	result += model_result

setMultithreading(0)
clearAll()
dp = getDisplayProperties()
dp.enableCreationForNewMolecules(0)
dp.setDrawingPrecision(DRAWING_PRECISION_HIGH)
dp.setSurfaceDrawingPrecision(6.5)
dp.selectMode(DRAWING_MODE_SOLID)
dp.selectColoringMethod(COLORING_ELEMENT)
dp.setTransparency(0)
openFile(Path().find("structures/bpti.pdb"))

showOnlyScene()

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
result_str = "Result: "+str(result)+" BALLView OpenGL stones"
print result_str
getMainControl().setStatusbarText(result_str, 1);
setMultithreading(1)
