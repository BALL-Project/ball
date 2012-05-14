### the directory name ###
SET(DIRECTORY source/SCORING/FUNCTIONS)

### list all filenames of the directory here ###
SET(SOURCES_LIST
	rescoring.C
	rescoring1D.C
	rescoring3D.C
	rescoring4D.C
	amberEvaluation.C
	energeticEvaluation.C
	forceFieldEvaluation.C
	gridedMM.C
	MMScoring.C
	gridedPLP.C
	PLPScoring.C
	randomEvaluation.C
	PBScoring.C
	slickEnergy.C
	slickScore.C
)	

ADD_BALL_SOURCES("SCORING/FUNCTIONS" "${SOURCES_LIST}")
