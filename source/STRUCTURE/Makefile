DIRECTORY=STRUCTURE
THISLIB=$(LIBNAME)

SMILES_SOURCES= \
	smilesParser.C\
	smilesParserParser.C\
	smilesParserLexer.C

CPP_SOURCES= \
	molecularGraph.C\
	analyticalSES.C\
	connolly.C\
	defaultProcessors.C\
	fragmentDB.C\
	buildBondsProcessor.C\
	geometricProperties.C\
	geometricTransformations.C\
	graphEdge.C\
	graphFace.C\
	graphVertex.C\
	numericalSAS.C\
	peptides.C\
	peptideBuilder.C\
	radialDistributionFunction.C\
	reconstructFragmentProcessor.C\
	RDFIntegrator.C\
	RDFParameter.C\
	RDFSection.C\
	reducedSurface.C\
	residueChecker.C\
	rotamerLibrary.C\
	RSEdge.C\
	RSFace.C\
	RSVertex.C\
	SASEdge.C\
	SASFace.C\
	SASVertex.C\
	SESEdge.C\
	SESFace.C\
	SESVertex.C\
	secondaryStructureProcessor.C\
	smilesParser.C\
	smilesParserLexer.C\
	smilesParserParser.C\
	solventAccessibleSurface.C\
	solventExcludedSurface.C\
	structureMapper.C\
	surfaceProcessor.C\
	triangle.C\
	triangleEdge.C\
	trianglePoint.C\
	triangulatedSAS.C\
	triangulatedSES.C\
	triangulatedSurface.C\
	HBondProcessor.C\
	UCK.C

PARSER_PREFIX=SmilesParser

include ../common.mak
include .Dependencies

depend:	$(SMILES_SOURCES)

default_clean:	clean_flex_bison

clean_flex_bison:
	@-$(RM) smilesParserParser.C smilesParserLexer.C smilesParserParser.h

smilesParserParser.C: smilesParserParser.y
smilesParserParser.h: smilesParserParser.y

smilesParserLexer.o: smilesParserLexer.C smilesParserParser.C
	$(CXX) $(CXXFLAGS) $(ADD_CXXFLAGS) $(CPP_MODE_FLAGS) $(LIB_CXXFLAGS) $(BALL_INCLUDES) $(ADD_INCLUDES) -c $*.C -o $@

smilesParserParser.o: smilesParserParser.C
	$(CXX) $(CXXFLAGS) $(ADD_CXXFLAGS) $(CPP_MODE_FLAGS) $(LIB_CXXFLAGS) $(BALL_INCLUDES) $(ADD_INCLUDES) -c $*.C -o $@

