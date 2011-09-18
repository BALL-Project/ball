SET(DIRECTORY source/DATATYPE/GRAPH)

### list all filenames of the directory here ###
SET(SOURCES_LIST
	graphAlgorithms.C
	molecularGraph.C
	treeWidth.C
)

ADD_BALL_SOURCES("DATATYPE/GRAPH" "${SOURCES_LIST}")
