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

### add path to the filenames ###
SET(SOURCES)
FOREACH(i ${SOURCES_LIST})
	LIST(APPEND SOURCES ${DIRECTORY}/${i})
ENDFOREACH()

### pass source file list to the upper instance ###
SET(BALL_sources ${BALL_sources} ${SOURCES})

### source group definition ###
SOURCE_GROUP("Source Files\\ENERGY" FILES ${SOURCE})
