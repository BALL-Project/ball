### the directory name ###
SET(DIRECTORY source/SCORING/FUNCTIONS)

### list all filenames of the directory here ###
SET(SOURCES_LIST
	slickEnergy.C
	slickScore.C
)	

ADD_BALL_SOURCES("SCORING/FUNCTIONS" "${SOURCES_LIST}")
