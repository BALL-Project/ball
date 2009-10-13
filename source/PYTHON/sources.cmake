### list all filenames of the directory here ###
SET(SOURCES_LIST "")

IF (BALL_HAS_PYTHON)
	SET(SOURCES_LIST ${SOURCES_LIST} pyInterpreter.C)
ENDIF()

ADD_BALL_SOURCES("PYTHON" "${SOURCES_LIST}")
