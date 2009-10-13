### the directory name ###
SET(DIRECTORY source/MOLMEC/CHARMM)

### list all filenames of the directory here ###
SET(SOURCES_LIST
	charmm.C
	charmmBend.C
	charmmImproperTorsion.C
	charmmNonBonded.C
	charmmStretch.C
	charmmTorsion.C
)	

ADD_BALL_SOURCES("MOLMEC/CHARMM" "${SOURCES_LIST}")
