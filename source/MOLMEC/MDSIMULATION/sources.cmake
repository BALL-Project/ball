### the directory name ###
SET(DIRECTORY source/MOLMEC/MDSIMULATION)

### list all filenames of the directory here ###
SET(SOURCES_LIST
	molecularDynamics.C
	microCanonicalMD.C
	canonicalMD.C 
)	

ADD_BALL_SOURCES("MOLMEC/MDSIMULATION" "${SOURCES_LIST}")
