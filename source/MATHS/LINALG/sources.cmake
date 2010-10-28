### list all filenames of the directory here ###
SET(SOURCES_LIST
	eigenSolver.C
	linalgException.C
	matrix.C
	vector.C
)

IF(BALL_HAS_LAPACK)
	LIST(APPEND SOURCES_LIST
		matrixInverter.C
		SVDSolver.C
	)
ENDIF()

ADD_BALL_SOURCES("MATHS/LINALG" "${SOURCES_LIST}")
