### list all filenames of the directory here ###
SET(SOURCES_LIST
	eigenSolver.C
	linalgException.C
	matrix.C
	matrixInverter.C
	SVDSolver.C
	vector.C
)

ADD_BALL_SOURCES("MATHS/LINALG" "${SOURCES_LIST}")
