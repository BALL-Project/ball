dp = getDisplayProperties()
fd = MolecularFileDialog.getInstance(0)
sc = Scene.getInstance(0)
ms = MolecularStructure.getInstance(0)

getMainControl().getPrimitiveManager().setMultithreadingMode(0)
	
dp.setTransparency(0)
dp.setDrawingPrecision(DRAWING_PRECISION_HIGH)
dp.selectMode(DRAWING_MODE_SOLID)
dp.selectColoringMethod(COLORING_ELEMENT)
dp.selectModel(MODEL_CARTOON)

fd.openFile("/var/tmp/test.pdb")

sc.exportPNG("/var/tmp/test1.png")

ms.calculateSecondaryStructure()
sc.exportPNG("/var/tmp/test2.png")
quit
