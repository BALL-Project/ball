### the directory name ###
SET(DIRECTORY source/COMMON)

### list all filenames of the directory here ###
SET(SOURCES_LIST
	constants.C
	exception.C
	hash.C
	init.C
	logStream.C
	rtti.C
	version.C
)	

ADD_BALL_SOURCES("COMMON" "${SOURCES_LIST}")
