### the directory name ###
SET(DIRECTORY source/QSAR)

### list all filenames of the directory here ###
SET(SOURCES_LIST
	aromaticityProcessor.C
	connectivityBase.C
	connectivityDescriptors.C
	descriptor.C
	partialChargeBase.C
	partialChargeDescriptors.C
	partialChargeProcessor.C
	ringPerceptionProcessor.C
	simpleBase.C
	simpleDescriptors.C
	surfaceBase.C
	surfaceDescriptors.C
)	

ADD_BALL_SOURCES("QSAR" "${SOURCES_LIST}")
