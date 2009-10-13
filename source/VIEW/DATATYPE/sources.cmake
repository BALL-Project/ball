### list all filenames of the directory here ###
SET(SOURCES_LIST
	colorExtensions.C
	colorHSV.C
	colorRGBA.C
	colorMap.C
	colorUnit.C
	colorUnitHue.C
	vertex1.C
	vertex2.C
	dataset.C
	raytraceableGrid.C
	standardDatasets.C
)

### the list of all files requiring a moc run ###
SET(MOC_SOURCES_LIST
	dataset.C
	standardDatasets.C
)

ADD_VIEW_SOURCES("VIEW/DATATYPE" "${SOURCES_LIST}")

ADD_BALL_MOCFILES("VIEW/DATATYPE" "include/BALL/VIEW/DATATYPE" "${MOC_SOURCES_LIST}")

