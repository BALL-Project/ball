DIRECTORY=STRUCTURE
THISLIB=$(LIBNAME)

SMILES_SOURCES=$(BALL_HAS_FLEX_YACC:true=smilesParser.C smilesParserParser.C smilesParserLexer.C)

CPP_SOURCES= $(SMILES_SOURCES)\
	analyticalSES.C\
	BSDTree.C\
	connolly.C\
	defaultProcessors.C\
	fragmentDB.C\
	geometricProperties.C\
	geometricTransformations.C\
	graphEdge.C\
	graphFace.C\
	graphVertex.C\
	numericalSAS.C\
	radialDistributionFunction.C\
	RDFIntegrator.C\
	RDFParameter.C\
	RDFSection.C\
	reducedSurface.C\
	residueChecker.C\
	rotamerLibrary.C\
	RSEdge.C\
	RSFace.C\
	RSVertex.C\
	SESEdge.C\
	SESFace.C\
	SESVertex.C\
	solventExcludedSurface.C\
	structureMapper.C\
	surfaceProcessor.C\
	triangle.C\
	triangleEdge.C\
	trianglePoint.C\
	triangulatedSES.C\
	triangulatedSurface.C


include ../common.mak
include .Dependencies

depend:	$(SMILES_SOURCES)

default_clean:	clean_flex_bison

clean_flex_bison:
	@-$(RM) smilesParserParser.C smilesParserLexer.C smilesParserParser.h

smilesParserParser.C smilesParserParser.h: smilesParserParser.y

smilesParserLexer.o: smilesParserLexer.C
	$(CXX) $(CXXFLAGS) $(ADD_CXXFLAGS) $(CPP_MODE_FLAGS) $(LIB_CXXFLAGS) $(BALL_INCLUDES) $(ADD_INCLUDES) -c $*.C -o $@

smilesParserParser.o: smilesParserParser.C
	$(CXX) $(CXXFLAGS) $(ADD_CXXFLAGS) $(CPP_MODE_FLAGS) $(LIB_CXXFLAGS) $(BALL_INCLUDES) $(ADD_INCLUDES) -c $*.C -o $@
