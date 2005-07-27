pm = getMainControl().getPrimitiveManager()
dp = getDisplayProperties()
fd = MolecularFileDialog.getInstance(0)

result = 0
nr_runs = 5

def clearRepresentations():
	nr = pm.getNumberOfRepresentations()
	rl = pm.getRepresentations()
	i = 0
	while i < nr:
		getMainControl().remove(rl[i])
		i = i + 1

def runTest(name, model):
	global result
	run = 0
	model_result = 0
	timer = Timer();
	timer.start();
	while run < nr_runs
		dp.selectModel(model)
		dp.apply()
		run += 1
		clearRepresentations()
	model_result = timer.getClockTime()
	print name+" "+str(model_result)+" seconds"
	if model == MODEL_SE_SURFACE:
		model_result /= 8.0
	result += model_result

clearRepresentations()
pm.setMultithreadingMode(0)
dp.enableCreationForNewMolecules(0)
dp.setDrawingPrecision(DRAWING_PRECISION_HIGH)
dp.setSurfaceDrawingPrecision(6.5)
dp.selectMode(DRAWING_MODE_SOLID)
dp.selectColoringMethod(COLORING_ELEMENT)
dp.setTransparency(0)
fd.openFile(Path().find("structures/bpti.pdb"))

runTest("Lines", 		MODEL_LINES)
runTest("VDW",   		MODEL_VDW)
runTest("BAS",   		MODEL_BALL_AND_STICK)
runTest("Cartoon", 	MODEL_CARTOON)
runTest("SES", 			MODEL_SE_SURFACE)

print "Result: "+str(result)+" seconds"
result /= nr_runs
result = 3 / result
print "Result: "+str(result)+" BALLView stones"
pm.setMultithreadingMode(1)
