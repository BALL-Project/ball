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

### the list of all files requiring a moc run ###
SET(MOC_SOURCES_LIST "")

ADD_VIEW_SOURCES("VIEW/MODELS" "${SOURCES_LIST}")

ADD_BALL_MOCFILES("VIEW/MODELS" "include/BALL/VIEW/MODELS" "${MOC_SOURCES_LIST}")
