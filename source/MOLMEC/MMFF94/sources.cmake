### the directory name ###
SET(DIRECTORY source/MOLMEC/MMFF94)

### list all filenames of the directory here ###
SET(SOURCES_LIST
	MMFF94.C
	MMFF94StretchBend.C
	MMFF94Torsion.C
	MMFF94Parameters.C
	MMFF94OutOfPlaneBend.C
	MMFF94Processors.C
	MMFF94NonBonded.C
)	

ADD_BALL_SOURCES("MOLMEC/MMFF94" "${SOURCES_LIST}")
