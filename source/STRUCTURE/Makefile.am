include ../Make.rules.in

DIRECTORY=STRUCTURE

_objects_LINK= ../config/genobjects.sh $(DIRECTORY)

BUILT_SOURCES=smilesParserParser.C smilesParserLexer.C smilesParserParser.h

smilesParserParser.C: smilesParserParser.y
smilesParserParser.h: smilesParserParser.y
	
CURRENTDIR_SOURCES= \
	molecularGraph.C\
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
	peptides.C\
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
	SESEdge.C\
	SESFace.C\
	SESVertex.C\
	smilesParser.C\
	smilesParserLexer.C\
	smilesParserParser.C\
	solventExcludedSurface.C\
	structureMapper.C\
	surfaceProcessor.C\
	triangle.C\
	triangleEdge.C\
	trianglePoint.C\
	triangulatedSES.C\
	triangulatedSurface.C

clean-local:
	$(RM) -f smilesParserParser.C smilesParserLexer.C smilesParserParser.h
	
PARSER_PREFIX=SmilesParser

#depend:	$(SMILES_SOURCES)
