m = MainControl.getInstance(0)
pm = m.getPrimitiveManager()
py = PyWidget.getInstance(0)

result = 0
nr_runs = 10

def clearRepresentations():
	nr = pm.getNumberOfRepresentations()
	rl = pm.getRepresentations()
	i = 0
	while i < nr:
		m.remove(rl[i])
		i = i + 1

def runTest(name, model):
	run = 0
	model_result = 0
	while run < nr_runs and not py.toAbortScript():
		t = Timer()
		t.start()
		dp.selectModel(model)
		dp.applyButtonClicked()
		t.stop()
		model_result += t.getCPUTime()
		run += 1
		clearRepresentations()
	print name+" "+str(model_result)

fd = MolecularFileDialog.getInstance(0)
fd.openFile("bpti.pdb")
clearRepresentations()
dp = DisplayProperties.getInstance(0)
dp.setDrawingPrecision(DRAWING_PRECISION_HIGH)
clearRepresentations()

runTest("Lines", 		MODEL_LINES)
runTest("VDW",   		MODEL_VDW)
runTest("BAS",   		MODEL_BALL_AND_STICK)
runTest("Cartoon", 	MODEL_CARTOON)
runTest("SES", 			MODEL_SE_SURFACE)

result /= nr_runs
print "Result: "+str(result)
