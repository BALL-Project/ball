INCLUDE(source/DATATYPE/GRAPH/sources.cmake)

### the directory name ###
SET(DIRECTORY source/DATATYPE)

### list all filenames of the directory here ###
SET(SOURCES_LIST
	bitVector.C
	contourSurface.C
	hashGrid.C
	options.C
	regularExpression.C
	regularData1D.C
	regularData2D.C
	regularData3D.C
	string.C
)	

ADD_BALL_SOURCES("DATATYPE" "${SOURCES_LIST}")
