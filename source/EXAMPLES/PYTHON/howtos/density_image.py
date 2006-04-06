pdb = "asd.pdb"
density = "asd.omap"
# color of mesh: red, green, blue
color = ColorRGBA(1., 0, 0)
# value for isosurface
value = 1.
distance = 4.

clearAll()
getDisplayProperties().enableCreationForNewMolecules(0)
openFile(filename)
grid = getDatasetControl().addDSN6Grid(density)
getDatasetControl().computeIsoContourSurface(grid, color, value)
modify_dialog = ModifyRepresentationDialog.getInstance(0)
modify_dialog.setGrid(grid)
modify_dialog.setSplitRadius(distance)
modify_dialog.applySplit()
reps = getRepresentations()
reps[0].hide()
reps[0].update()
reps[1].setDrawingMode(DRAWING_MODE_WIREFRAME)
reps[1].setTransparency(120)
reps[1].update()
