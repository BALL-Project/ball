### the directory name ###
SET(DIRECTORY source/MOLMEC/MINIMIZATION)

### list all filenames of the directory here ###
SET(SOURCES_LIST
	energyMinimizer.C
	lineSearch.C
	steepestDescent.C
	strangLBFGS.C
	shiftedLVMM.C
	conjugateGradient.C
)	

ADD_BALL_SOURCES("MOLMEC/MINIMIZATION" "${SOURCES_LIST}")
