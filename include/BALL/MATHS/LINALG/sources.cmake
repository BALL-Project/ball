### list all filenames of the directory here ###
SET(GROUP MATHS/LINALG)

FILE(GLOB HEADERS_LIST "include/BALL/${GROUP}/*.h" "include/BALL/${GROUP}/*.iC" "include/BALL/${GROUP}/*.ih")	

IF(NOT BALL_HAS_LAPACK)
	LIST(REMOVE_ITEM HEADERS_LIST
		"include/BALL/${GROUP}/matrixInverter.h"
		"include/BALL/${GROUP}/matrixInverter.iC"
		"include/BALL/${GROUP}/matrixInverter.ih"
		"include/BALL/${GROUP}/SVDSolver.h"
		"include/BALL/${GROUP}/SVDSolver.iC"
		"include/BALL/${GROUP}/SVDSolver.ih"
	)
ENDIF()

ADD_BALL_HEADERS("${GROUP}" "${HEADERS_LIST}")
