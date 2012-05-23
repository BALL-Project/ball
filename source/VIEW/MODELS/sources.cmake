### list all filenames of the directory here ###
SET(SOURCES_LIST
	atomBondModelBaseProcessor.C
	backboneModel.C
	ballAndStickModel.C
	cartoonModel.C
	colorByGridProcessor.C
	colorProcessor.C
	fieldLineCreator.C
	forceModel.C
	HBondModel.C
	labelModel.C
	lineModel.C
	modelProcessor.C
	standardColorProcessor.C
	surfaceModel.C
	vanDerWaalsModel.C
)

ADD_VIEW_SOURCES("VIEW/MODELS" "${SOURCES_LIST}")
