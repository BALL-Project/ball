### the directory name ###
SET(DIRECTORY source/ENERGY)

### list all filenames of the directory here ###
SET(SOURCES_LIST
	atomicContactEnergy.C
	composedEnergyProcessor.C
	coulomb.C
	distanceCoulomb.C
	energyProcessor.C
)	

ADD_BALL_SOURCES("ENERGY" "${SOURCES_LIST}")
