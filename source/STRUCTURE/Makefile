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
-include .Dependencies

depend:	$(SMILES_SOURCES)

smilesParserParser.C:	smilesParserParser.y
	$(YACC) -d smilesParserParser.y &&\
		$(MV) y.tab.h smilesParserParser.h &&\
		$(MV) y.tab.c smilesParserParser.C

smilesParserLexer.C: smilesParserLexer.l smilesParserParser.C
	$(LEX) -osmilesParserLexer.C smilesParserLexer.l
