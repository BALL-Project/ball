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

### the list of all files requiring a moc run ###
SET(MOC_SOURCES_LIST "")

ADD_VIEW_SOURCES("VIEW/PRIMITIVES" "${SOURCES_LIST}")

ADD_BALL_MOCFILES("VIEW/PRIMITIVES" "include/BALL/VIEW/PRIMITIVES" "${MOC_SOURCES_LIST}")
