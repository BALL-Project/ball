include ../common.mak

DIRECTORY=STRUCTURE
THISLIB=$(LIBNAME)

DIRS=\
	DOCKING


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
	UCK.C\
	md5.C

PARSER_PREFIX=SmilesParser

include .Dependencies

default: echodir $(OBJECTS) dirs

depend:	$(SMILES_SOURCES) subdirsdepend

default_clean:	clean_flex_bison subdirsclean

collectlib: subdirscollectlib

dirs: dummy
	@$(MAKE) subdirs SUBDIRS="$(DIRS)" TARGET=default

subdirs:	dummy
	@set -e; for i in $(SUBDIRS); do cd $$i; if $(MAKE) $(TARGET); then cd .. ; else cd .. ; exit 1 ; fi ; done

subdirsdepend:	dummy
	@echo "" > .Dependencies
	@for i in $(DIRS); do ( cd $$i; echo "" >.Dependencies; cd .. ) ; done
	@$(MAKE) subdirs SUBDIRS="$(DIRS)" TARGET=depend

subdirsclean:
	@$(MAKE) subdirs SUBDIRS="$(DIRS)" TARGET=default_clean

subdirscollectlib:	dummy
	@$(MAKE) subdirs SUBDIRS="$(DIRS)" TARGET=collectlib LIBNAME="$(THISLIB)"


clean_flex_bison:
	@-$(RM) smilesParserParser.C smilesParserLexer.C smilesParserParser.h

smilesParserParser.C: smilesParserParser.y
smilesParserParser.h: smilesParserParser.y

smilesParserLexer.o: smilesParserLexer.C smilesParserParser.C
	$(CXX) $(CXXFLAGS) $(ADD_CXXFLAGS) $(CPP_MODE_FLAGS) $(LIB_CXXFLAGS) $(BALL_INCLUDES) $(ADD_INCLUDES) -c $*.C -o $@

smilesParserParser.o: smilesParserParser.C
	$(CXX) $(CXXFLAGS) $(ADD_CXXFLAGS) $(CPP_MODE_FLAGS) $(LIB_CXXFLAGS) $(BALL_INCLUDES) $(ADD_INCLUDES) -c $*.C -o $@

dummy:
