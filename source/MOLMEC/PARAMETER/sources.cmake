### the directory name ###
SET(DIRECTORY source/MOLMEC/PARAMETER)

### list all filenames of the directory here ###
SET(SOURCES_LIST
	atomTypes.C
	charmmEEF1.C
	cosineTorsion.C
	forceFieldParameters.C
	lennardJones.C
	potential1210.C
	quadraticAngleBend.C
	quadraticBondStretch.C
	quadraticImproperTorsion.C
	residueTorsions.C
	templates.C
)	

ADD_BALL_SOURCES("MOLMEC/PARAMETER" "${SOURCES_LIST}")
