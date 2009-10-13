### the directory name ###
SET(DIRECTORY source/SCORING/COMMON)

### list all filenames of the directory here ###
SET(SOURCES_LIST
	baseFunction.C
	linearBaseFunction.C
	fermiBaseFunction.C
	scoringComponent.C
	scoringFunction.C
	scoringTypes.C
)	

ADD_BALL_SOURCES("SCORING/COMMON" "${SOURCES_LIST}")
