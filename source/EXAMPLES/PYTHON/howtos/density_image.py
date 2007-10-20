pdb = "/local/amoll/scheidig/Strukturbilder_2006_21_03_06/data/p14/p14_50_1.pdb"
density = "/local/amoll/scheidig/Strukturbilder_2006_21_03_06/data/p14/1fofc_p14_50.omap"
#pdb = "/big/incoming/p14/p14_50_1.pdb"
#density = "/big/incoming/p14/1fofc_p14_50.omap"
# color of mesh: red, green, blue
color = ColorRGBA(1., 0, 0)
# value for isosurface
value = 0.7
distance = 1.7

getDisplayProperties().enableCreationForNewMolecules(0)
openFile(pdb)
getMolecularControl().applySelector("residueID(17) OR residueID(33-36) OR residueID(60-66) OR residueID(200)")

grid = getDatasetControl().addDSN6Grid(density)
getDatasetControl().computeIsoContourSurface(grid, color, value)
if len(getRepresentations()) == 0:
	getMainControl().setStatusbarText("No surface could be generated for given value", 1)
	abortScript()

modify_dialog = ModifyRepresentationDialog.getInstance(0)
cs = getRepresentations()[len(getRepresentations()) - 1]
modify_dialog.setRepresentation(cs)
modify_dialog.setSplitRadius(distance)
modify_dialog.applySplit()
reps = getRepresentations()
nr = len(getRepresentations())
reps[nr - 2].setHidden(true)
reps[nr - 1].setDrawingMode(DRAWING_MODE_WIREFRAME)
reps[nr - 1].setTransparency(120)
getMainControl().redrawAllRepresentations(1)
