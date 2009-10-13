### the directory name ###
SET(DIRECTORY source/SCORING/COMPONENTS)

### list all filenames of the directory here ###
SET(SOURCES_LIST
	aromaticRingStacking.C
	buriedPolar.C
	CHPI.C
	hydrogenBond.C
	lipophilic.C
	nonpolarSolvation.C
	polarSolvation.C
	rotationalEntropyLoss.C
	vanDerWaals.C
)	

ADD_BALL_SOURCES("SCORING/COMPONENTS" "${SOURCES_LIST}")
