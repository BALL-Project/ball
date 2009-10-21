### list all filenames of the directory here ###
SET(SOURCES_LIST
	crystalGenerator.C
	crystalInfo.C
	spaceGroupParameters.C
)

ADD_BALL_SOURCES("XRAY" "${SOURCES_LIST}")
