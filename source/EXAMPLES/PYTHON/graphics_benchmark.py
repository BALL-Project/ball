result = 0
nr_runs = 5

def runTest(name, model):
	global result
	model_result = 0
	timer = Timer()
	timer.start()
	for i in range(0, nr_runs):
		getDisplayProperties().selectModel(model)
		getDisplayProperties().createRepresentation(getSystems())
		clearRepresentations()
	model_result = timer.getClockTime()
	print name+" "+str(model_result)+" seconds"
	if model == MODEL_SE_SURFACE:
		model_result /= 8.0
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

runTest("Lines", 		MODEL_LINES)
runTest("VDW",   		MODEL_VDW)
runTest("BAS",   		MODEL_BALL_AND_STICK)
runTest("Cartoon", 	MODEL_CARTOON)
runTest("SES", 			MODEL_SE_SURFACE)

print "Result: "+str(result)+" seconds"
result /= nr_runs
result = 3 / result
result_str = "Result: "+str(result)+" BALLView stones"
print result_str
getMainControl().setStatusbarText(result_str, 1);
setMultithreading(1)
dp.enableCreationForNewMolecules(0)

