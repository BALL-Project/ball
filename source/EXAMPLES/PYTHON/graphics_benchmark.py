m = MainControl.getInstance(0)
pm = m.getPrimitiveManager()
py = PyWidget.getInstance(0)
dp = DisplayProperties.getInstance(0)
fd = MolecularFileDialog.getInstance(0)

result = 0
nr_runs = 5

def clearRepresentations():
	nr = pm.getNumberOfRepresentations()
	rl = pm.getRepresentations()
	i = 0
	while i < nr:
		m.remove(rl[i])
		i = i + 1

def runTest(name, model):
	global result
	run = 0
	model_result = 0
	timer = Timer();
	timer.start();
	while run < nr_runs and not py.toAbortScript():
		dp.selectModel(model)
		dp.apply()
		run += 1
		clearRepresentations()
	model_result = timer.getClockTime()
	print name+" "+str(model_result)+" seconds"
	result += model_result

clearRepresentations()
m.getPrimitiveManager().setMultithreadingMode(0)
dp.enableCreationForNewMolecules(0)
dp.setDrawingPrecision(DRAWING_PRECISION_HIGH)
dp.setSurfaceDrawingPrecision(6.5)
dp.selectMode(DRAWING_MODE_SOLID)
dp.selectColoringMethod(COLORING_ELEMENT)
dp.setTransparency(0)
fd.openFile("bpti.pdb")

runTest("Lines", 		MODEL_LINES)
runTest("VDW",   		MODEL_VDW)
runTest("BAS",   		MODEL_BALL_AND_STICK)
runTest("Cartoon", 	MODEL_CARTOON)
runTest("SES", 			MODEL_SE_SURFACE)

print "Result: "+str(result)+" seconds"
result /= nr_runs
result = 10 / result
print "Result: "+str(result)+" BALLView stones"
m.getPrimitiveManager().setMultithreadingMode(1)
