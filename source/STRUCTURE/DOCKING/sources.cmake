### list all filenames of the directory here ###
SET(SOURCES_LIST
	dockResult.C
	dockingAlgorithm.C
	conformationSet.C
	energeticEvaluation.C
	forceFieldEvaluation.C
	amberEvaluation.C
	randomEvaluation.C
)

IF (BALL_HAS_FFTW)
	SET(SOURCES_LIST ${SOURCES_LIST} geometricFit.C)
ENDIF()

ADD_BALL_SOURCES("STRUCTURE/DOCKING" "${SOURCES_LIST}")
