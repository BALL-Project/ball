### the directory name ###
SET(DIRECTORY source/DOCKING/COMMON)

### list all filenames of the directory here ###
SET(SOURCES_LIST
	conformation.C
	constraints.C
	conformationSet.C
	dockingAlgorithm.C
	dockResult.C
	flexDefinition.C
	flexibleMolecule.C
	gridAnalysis.C
	poseClustering.C
	receptor.C
	result.C
	sideChainOptimizer.C
	staticLigandFragment.C
	structurePreparer.C
)

ADD_BALL_SOURCES("DOCKING/COMMON" "${SOURCES_LIST}")
