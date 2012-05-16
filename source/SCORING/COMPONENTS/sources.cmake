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
	CHPISlick.C
	electrostatic.C
	fresnoTypes.C
	hydrogenBond.C
	hydrogenBondSlick.C
	lipophilic.C
	polarity.C
	PB.C
	PLP.C
	rotationalEntropy.C
	solvation.C
	nonpolarSolvation.C
	polarSolvation.C
	#rotationalEntropyLoss.C
	vanDerWaals.C
	vanDerWaalsSlick.C
)

ADD_BALL_SOURCES("SCORING/COMPONENTS" "${SOURCES_LIST}")
