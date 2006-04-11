pdb = "/big/incoming/p14/p14_50_1.pdb"
density = "/big/incoming/p14/1fofc_p14_50.omap"
# color of mesh: red, green, blue
color = ColorRGBA(1., 0, 0)
# value for isosurface
value = 0.7
distance = 1.7

getDisplayProperties().enableCreationForNewMolecules(0)
openFile(pdb)
grid = getDatasetControl().addDSN6Grid(density)
getDatasetControl().computeIsoContourSurface(grid, color, value)
modify_dialog = ModifyRepresentationDialog.getInstance(0)
modify_dialog.setGrid(grid)
modify_dialog.setSplitRadius(distance)
modify_dialog.applySplit()
reps = getRepresentations()
nr = len(getRepresentations())
reps[nr - 2].setHidden(true)
reps[nr - 2].update()
reps[nr - 1].setDrawingMode(DRAWING_MODE_WIREFRAME)
reps[nr - 1].setTransparency(120)
reps[nr - 1].update()
