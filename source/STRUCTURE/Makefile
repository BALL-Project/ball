# -*- Mode: C++; tab-width: 2; -*-
# vi: set ts=2:
#

include ../config.mak

DIRECTORY=STRUCTURE

THISLIB=$(LIBNAME)

STRUCTURE_SUBDIRS=\
	DOCKING

SMILES_SOURCES= \
	smilesParser.C\
	smilesParserParser.C\
	smilesParserLexer.C

SMARTS_SOURCES=\
	smartsParser.C\
	smartsParserParser.C\
	smartsParserLexer.C

CPP_SOURCES=\
	addHydrogenProcessor.C\
	analyticalSES.C\
	atomBijection.C\
	atomTyper.C\
	bindingPocketProcessor.C\
	buildBondsProcessor.C\
	connectedComponentsProcessor.C\
	connolly.C\
	defaultProcessors.C\
	DNAMutator.C\
	fragmentDB.C\
	geometricProperties.C\
	geometricTransformations.C\
	graphEdge.C\
	graphFace.C\
	graphVertex.C\
	HBondProcessor.C\
	kekulizer.C\
	moleculeAssembler.C\
	molecularGraph.C\
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
	ringAnalyser.C\
	ringClusterer.C\
	RSEdge.C\
	RSFace.C\
	RSVertex.C\
	SASEdge.C\
	SASFace.C\
	SASVertex.C\
	sdGenerator.C\
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
	smartsMatcher.C\
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
	UCK.C\
	residueRotamerSet.C\
	rotamerLibrary.C\
	RMSDMinimizer.C

ifeq ($(BALL_HAS_VIEW),true)
	CPP_SOURCES+=\
	hybridisationProcessor.C\
	assignBondOrderProcessor.C
endif

SMILES_PARSER_PREFIX=SmilesParser
SMARTS_PARSER_PREFIX=SmartsParser

include ../common.mak
-include .Dependencies

depend:	$(SMILES_SOURCES)	$(SMARTS_SOURCES) subdirsdepend

default: echodir $(OBJECTS) dirs

default_clean:	clean_parser subdirsclean

collectlib: subdirscollectlib

dirs: dummy
	@$(MAKE) subdirs SUBDIRS="$(STRUCTURE_SUBDIRS)" TARGET=default

subdirs:	dummy
	@set -e; for i in $(SUBDIRS); do cd $$i; if $(MAKE) $(TARGET); then cd .. ; else cd .. ; exit 1 ; fi ; done

subdirsdepend:	dummy
	@echo "" > .Dependencies
	@for i in $(STRUCTURE_SUBDIRS); do ( cd $$i; echo "" >.Dependencies; cd .. ) ; done
	@$(MAKE) subdirs SUBDIRS="$(STRUCTURE_SUBDIRS)" TARGET=depend

subdirsclean:
	@$(MAKE) subdirs SUBDIRS="$(STRUCTURE_SUBDIRS)" TARGET=default_clean

subdirscollectlib:	dummy
	@$(MAKE) subdirs SUBDIRS="$(STRUCTURE_SUBDIRS)" TARGET=collectlib LIBNAME="$(THISLIB)"


clean_parser:
	@-$(RM) smilesParserParser.C smilesParserLexer.C smilesParserParser.h smartsParserParser.C smartsParserLexer.C smartsParserParser.h *.bak 2>/dev/null

smilesParserParser.C: smilesParserParser.y
smilesParserParser.h: smilesParserParser.y

smilesParserLexer.C:  smilesParserLexer.l
	$(LEX)	-P$(SMILES_PARSER_PREFIX)	-o$@ $*.l

smilesParserParser.C: smilesParserParser.y
	$(YACC) -p $(SMILES_PARSER_PREFIX) -d $*.y && $(MV) y.tab.h $*.h && $(MV) y.tab.c $@

smilesParserLexer.o: smilesParserLexer.C smilesParserParser.C
	$(CXX) $(CXXFLAGS) $(ADD_CXXFLAGS) $(CPP_MODE_FLAGS) $(LIB_CXXFLAGS) $(BALL_INCLUDES) $(ADD_INCLUDES) -c $*.C -o $@

smilesParserParser.o: smilesParserParser.C
	$(CXX) $(CXXFLAGS) $(ADD_CXXFLAGS) $(CPP_MODE_FLAGS) $(LIB_CXXFLAGS) $(BALL_INCLUDES) $(ADD_INCLUDES) -c $*.C -o $@ || \
	../config/fixbison smilesParserParser.C

# Add smiles parser dependencies to make sure these two won't be
# build in parallel when calling make -j 
smartsParserParser.C: smartsParserParser.y smilesParserParser.C
smartsParserParser.h:	smartsParserParser.y smilesParserParser.h

smartsParserLexer.C:	smartsParserLexer.l smilesParserLexer.C
	$(LEX)  -P$(SMARTS_PARSER_PREFIX) -o$@ $*.l

smartsParserParser.C: smartsParserParser.y
# this line is just for debugging the parser
#	$(YACC) -p $(SMARTS_PARSER_PREFIX) --report=state --report=look-ahead -g -d $*.y && $(MV) y.tab.h $*.h && $(MV) y.tab.c $@
	$(YACC) -p $(SMARTS_PARSER_PREFIX) -d $*.y && $(MV) y.tab.h $*.h && $(MV) y.tab.c $@

smartsParserLexer.o: smartsParserLexer.C smartsParserParser.C
	$(CXX) $(CXXFLAGS) $(ADD_CXXFLAGS) $(CPP_MODE_FLAGS) $(LIB_CXXFLAGS) $(BALL_INCLUDES) $(ADD_INCLUDES) -c $*.C -o $@

smartsParserParser.o: smartsParserParser.C
	$(CXX) $(CXXFLAGS) $(ADD_CXXFLAGS) $(CPP_MODE_FLAGS) $(LIB_CXXFLAGS) $(BALL_INCLUDES) $(ADD_INCLUDES) -c $*.C -o $@ || \
	../config/fixbison smartsParserParser.C



dummy:
