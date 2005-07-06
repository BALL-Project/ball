DIRECTORY=STRUCTURE
THISLIB=$(LIBNAME)

SMILES_SOURCES= \
	smilesParser.C\
	smilesParserParser.C\
	smilesParserLexer.C

SMARTS_SOURCES=\
	smartsParser.C\
	smartsParserParser.C\
	smartsParserLexer.C

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
	smartsParser.C\
	smartsParserLexer.C\
	smartsParserParser.C\
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

SMILES_PARSER_PREFIX=SmilesParser
SMARTS_PARSER_PREFIX=SmartsParser

include ../common.mak
-include .Dependencies

depend:	\
	$(SMILES_SOURCES)\
	$(SMARTS_SOURCES)

default_clean:	clean_parser

clean_parser:
	@-$(RM) smilesParserParser.C smilesParserLexer.C smilesParserParser.h smartsParserParser.C smartsParserLexer.C smartsParserParser.h

smilesParserParser.C: smilesParserParser.y
smilesParserParser.h: smilesParserParser.y

smilesParserLexer.C:  smilesParserLexer.l
	$(LEX)	-P$(SMILES_PARSER_PREFIX)	-o$@ $*.l

smilesParserParser.C: smilesParserParser.y
	$(YACC) -p $(SMILES_PARSER_PREFIX) -d $*.y && $(MV) y.tab.h $*.h && $(MV) y.tab.c $@

smilesParserLexer.o: smilesParserLexer.C smilesParserParser.C
	$(CXX) $(CXXFLAGS) $(ADD_CXXFLAGS) $(CPP_MODE_FLAGS) $(LIB_CXXFLAGS) $(BALL_INCLUDES) $(ADD_INCLUDES) -c $*.C -o $@

smilesParserParser.o: smilesParserParser.C
	$(CXX) $(CXXFLAGS) $(ADD_CXXFLAGS) $(CPP_MODE_FLAGS) $(LIB_CXXFLAGS) $(BALL_INCLUDES) $(ADD_INCLUDES) -c $*.C -o $@

# Add smiles parser dependencies to make sure these two won't be
# build in parallel when calling make -j 
smartsParserParser.C: smartsParserParser.y smilesParserParser.C
smartsParserParser.h:	smartsParserParser.y smilesParserParser.h

smartsParserLexer.C:	smartsParserLexer.l smilesParserLexer.C
	$(LEX)  -P$(SMARTS_PARSER_PREFIX) -o$@ $*.l

smartsParserParser.C: smartsParserParser.y
	$(YACC) -p $(SMARTS_PARSER_PREFIX) -d $*.y && $(MV) y.tab.h $*.h && $(MV) y.tab.c $@

smartsParserLexer.o: smartsParserLexer.C smartsParserParser.C
	$(CXX) $(CXXFLAGS) $(ADD_CXXFLAGS) $(CPP_MODE_FLAGS) $(LIB_CXXFLAGS) $(BALL_INCLUDES) $(ADD_INCLUDES) -c $*.C -o $@

smartsParserParser.o: smartsParserParser.C
	$(CXX) $(CXXFLAGS) $(ADD_CXXFLAGS) $(CPP_MODE_FLAGS) $(LIB_CXXFLAGS) $(BALL_INCLUDES) $(ADD_INCLUDES) -c $*.C -o $@

