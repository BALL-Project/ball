### the directory name ###
SET(DIRECTORY source/SCORING/COMMON)

### list all filenames of the directory here ###
SET(SOURCES_LIST
	baseFunction.C
	diffScoringFunction.C
	diffGridBasedScoring.C
	gridBasedScoring.C
	linearBaseFunction.C
	fermiBaseFunction.C
	rescorer.C
	scoreGridSet.C
	scoringComponent.C
	scoringFunction.C
	scoringOptimizer.C
)	

ADD_BALL_SOURCES("SCORING/COMMON" "${SOURCES_LIST}")
