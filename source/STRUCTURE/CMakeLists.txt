set(path STRUCTURE)


SET(YACC_SOURCES "")
SET(YACC_SOURCES 
	smartsParserParser.y
	smilesParserParser.y
)

foreach(i ${YACC_SOURCES})
	wrap_YACC(BALL ${path} ${i})
endforeach(i)


SET(FLEX_SOURCES "")
SET(FLEX_SOURCES 
	smartsParserLexer.l
	smilesParserLexer.l
)

foreach(i ${FLEX_SOURCES})
	wrap_FLEX(BALL ${path} ${i})
endforeach(i)


list(APPEND BALL_SOURCES 
	STRUCTURE/addHydrogenProcessor.C
	STRUCTURE/analyticalSES.C
	STRUCTURE/assignBondOrderProcessor.C
	STRUCTURE/atomBijection.C
	STRUCTURE/atomTyper.C
	STRUCTURE/bindingPocketProcessor.C
	STRUCTURE/buildBondsProcessor.C
	STRUCTURE/chainBuilder.C
	STRUCTURE/connolly.C
	STRUCTURE/defaultProcessors.C
	STRUCTURE/DNAMutator.C
	STRUCTURE/fragmentDB.C
	STRUCTURE/geometricProperties.C
	STRUCTURE/geometricTransformations.C
	STRUCTURE/graphEdge.C
	STRUCTURE/graphFace.C
	STRUCTURE/graphVertex.C
	STRUCTURE/HBondProcessor.C
	STRUCTURE/hybridisationProcessor.C
	STRUCTURE/kekulizer.C
	STRUCTURE/molecularGraph.C
	STRUCTURE/moleculeAssembler.C
	STRUCTURE/numericalSAS.C
	STRUCTURE/peptideBuilder.C
	STRUCTURE/peptides.C
	STRUCTURE/radialDistributionFunction.C
	STRUCTURE/RDFIntegrator.C
	STRUCTURE/RDFParameter.C
	STRUCTURE/RDFSection.C
	STRUCTURE/reconstructFragmentProcessor.C
	STRUCTURE/reducedSurface.C
	STRUCTURE/residueChecker.C
	STRUCTURE/residueRotamerSet.C
	STRUCTURE/ringAnalyser.C
	STRUCTURE/ringClusterer.C
	STRUCTURE/RMSDMinimizer.C
	STRUCTURE/rotamerLibrary.C
	STRUCTURE/rsConstructor.C
	STRUCTURE/RSEdge.C
	STRUCTURE/RSFace.C
	STRUCTURE/RSVertex.C
	STRUCTURE/SASEdge.C
	STRUCTURE/SASFace.C
	STRUCTURE/SASVertex.C
	STRUCTURE/sdGenerator.C
	STRUCTURE/secondaryStructureProcessor.C
	STRUCTURE/SESEdge.C
	STRUCTURE/SESFace.C
	STRUCTURE/SESVertex.C
	STRUCTURE/smartsMatcher.C
	STRUCTURE/smartsParser.C
	STRUCTURE/smilesParser.C
	STRUCTURE/solventAccessibleSurface.C
	STRUCTURE/solventExcludedSurface.C
	STRUCTURE/structureMapper.C
	STRUCTURE/surfaceProcessor.C
	STRUCTURE/triangle.C
	STRUCTURE/triangleEdge.C
	STRUCTURE/trianglePoint.C
	STRUCTURE/triangulatedSAS.C
	STRUCTURE/triangulatedSES.C
	STRUCTURE/triangulatedSurface.C
	STRUCTURE/UCK.C
)

