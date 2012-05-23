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

ADD_VIEW_SOURCES("VIEW/DATATYPE" "${SOURCES_LIST}")
