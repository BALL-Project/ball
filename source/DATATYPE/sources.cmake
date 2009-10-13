### the directory name ###
SET(DIRECTORY source/DATATYPE)

### list all filenames of the directory here ###
SET(SOURCES_LIST
	bitVector.C
	string.C
	hashGrid.C
	options.C
	regularExpression.C
	contourSurface.C
	regularData1D.C
	regularData2D.C
	regularData3D.C
)	

ADD_BALL_SOURCES("DATATYPE" "${SOURCES_LIST}")
