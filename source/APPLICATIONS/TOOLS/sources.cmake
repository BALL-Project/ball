### the directory name
SET(DIRECTORY source/APPLICATIONS/TOOLS)

SET(EXECUTABLES_LIST
	AntitargetRescorer
	AutoModel
	BindingDBCleaner
	CombiLibGenerator
	ConstraintsFinder
	Converter
	DBExporter
	DBImporter
	DockResultMerger
	EvenSplit
	FeatureSelector
	GalaxyConfigGenerator
	GridBuilder
	IMGDock
	InputPartitioner
	InputReader
	InteractionConstraintDefiner
	LigandFileSplitter
	LigCheck
	ModelCreator
	MolCombine
	MolFilter
	MolPredictor
	PartialChargesCopy
	PDBCutter
	PDBDownload
	PocketDetector
	Predictor
	PropertyModifier
	PropertyPlotter
	ProteinCheck
	RMSDCalculator
	ScoreAnalyzer
	SideChainGridBuilder
	SimilarityAnalyzer
	SimpleRescorer
	SLICK
	SpatialConstraintDefiner
	TaGRes
	TaGRes-train
	Validator
	VendorFinder
	WaterFinder
	# add new programs here
)

IF(BALL_HAS_OPENBABEL OR BALL_HAS_OEPNEYE)
	LIST(APPEND EXECUTABLES_LIST
		Ligand3DGenerator
		MolDepict
		ProteinProtonator
	)
ENDIF()

SET(TOOLS_EXECUTABLES ${TOOLS_EXECUTABLES} ${EXECUTABLES_LIST})

### add filenames to Visual Studio solution
SET(TOOLS_SOURCES)
FOREACH(i ${EXECUTABLES_LIST})
	LIST(APPEND TOOLS_SOURCES "${i}")
ENDFOREACH(i)
SOURCE_GROUP("" FILES ${TOOLS_SOURCES})
