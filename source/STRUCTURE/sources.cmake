INCLUDE(source/STRUCTURE/BONDORDERS/sources.cmake)

### list all filenames of the directory here ###
SET(SOURCES_LIST
	addHydrogenProcessor.C
	analyticalSES.C
	assignBondOrderProcessor.C
	atomBijection.C
	atomTyper.C
	binaryFingerprintMethods.C
	bindingPocketProcessor.C
	buildBondsProcessor.C
	connectedComponentsProcessor.C
	connolly.C
	defaultProcessors.C
	disulfidBondProcessor.C
	DNAMutator.C
	fragmentDB.C
	geometricProperties.C
	geometricTransformations.C
	graphEdge.C
	graphFace.C
	graphVertex.C
	HBondProcessor.C
	hybridisationProcessor.C
	kekulizer.C
	molecularSimilarity.C
	mutator.C
	nucleotideMapping.C
	numericalSAS.C
	peptides.C
	peptideBuilder.C
	peptideCapProcessor.C
	radialDistributionFunction.C
	reconstructFragmentProcessor.C
	RDFIntegrator.C
	RDFParameter.C
	RDFSection.C
	reducedSurface.C
	residueChecker.C
	rGroupAssembler.C
	ringAnalyser.C
	RSEdge.C
	RSFace.C
	RSVertex.C
	SASEdge.C
	SASFace.C
	SASVertex.C
	sdGenerator.C
	SESEdge.C
	SESFace.C
	SESVertex.C
	secondaryStructureProcessor.C
	sideChainPlacementProcessor.C
	smilesParser.C
	smartsParser.C
	smartsMatcher.C
	solventAccessibleSurface.C
	solventExcludedSurface.C
	structureMapper.C
	surfaceProcessor.C
	triangle.C
	triangleEdge.C
	trianglePoint.C
	triangulatedSAS.C
	triangulatedSES.C
	triangulatedSurface.C
	UCK.C
	residueRotamerSet.C
	rotamerLibrary.C
	RMSDMinimizer.C
)

IF(BALL_HAS_OPENBABEL)
	LIST(APPEND SOURCES_LIST logP.C)
ENDIF()

ADD_BALL_SOURCES("STRUCTURE" "${SOURCES_LIST}")

ADD_BALL_PARSER_LEXER("STRUCTURE" "smilesParser" "SmilesParser")
ADD_BALL_PARSER_LEXER("STRUCTURE" "smartsParser" "SmartsParser")
