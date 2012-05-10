### the directory name ###
SET(DIRECTORY source/SCORING/COMPONENTS)

### list all filenames of the directory here ###
SET(SOURCES_LIST
	aromaticRingStacking.C
	advElectrostatic.C
	fragmentationalSolvation.C
	burialDepth.C
	buriedPolar.C
	CHPI.C
	electrostatic.C
	fresnoTypes.C
	hydrogenBond.C
	lipophilic.C
	polarity.C
	PB.C
	PLP.C
	rotationalEntropy.C
	solvation.C
	#nonpolarSolvation.C
	#polarSolvation.C
	#rotationalEntropyLoss.C
	#vanDerWaals should be included again, right now there are unresolved compilation problems
	vanDerWaalsSlick.C
	vanDerWaals.C
	# vdW.C should be merged into other files. it is a wrapper for amberNonBonded
	#vdW.C
)	

ADD_BALL_SOURCES("SCORING/COMPONENTS" "${SOURCES_LIST}")
