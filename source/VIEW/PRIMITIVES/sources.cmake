### list all filenames of the directory here ###
SET(SOURCES_LIST
	box.C
	disc.C
	gridVisualisation.C
	label.C
	line.C
	mesh.C
	multiLine.C
	quadMesh.C
	point.C
	simpleBox.C
	sphere.C
	tube.C
	twoColoredLine.C
	twoColoredTube.C
)

ADD_VIEW_SOURCES("VIEW/PRIMITIVES" "${SOURCES_LIST}")
