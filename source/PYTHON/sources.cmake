### list all filenames of the directory here ###
SET(SOURCES_LIST "")

IF (BALL_PYTHON_SUPPORT)
	SET(SOURCES_LIST ${SOURCES_LIST} pyInterpreter.C)
ENDIF()

ADD_BALL_SOURCES("PYTHON" "${SOURCES_LIST}")
