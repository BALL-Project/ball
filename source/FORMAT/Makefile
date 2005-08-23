DIRECTORY=FORMAT

THISLIB=$(LIBNAME)

PARSER_SOURCES= \
	GAMESSDatFile.C\
	GAMESSDatParserParser.C\
	GAMESSDatParserLexer.C\
	GAMESSLogFile.C\
	GAMESSLogParserParser.C\
	GAMESSLogParserLexer.C


CPP_SOURCES=\
	bruker1DFile.C\
	bruker2DFile.C\
	DCDFile.C\
	genericMolFile.C\
	GAMESSDatFile.C\
	GAMESSDatParserParser.C\
	GAMESSDatParserLexer.C\
	GAMESSLogFile.C\
	GAMESSLogParserParser.C\
	GAMESSLogParserLexer.C\
	HINFile.C\
	INIFile.C\
	JCAMPFile.C\
	KCFFile.C\
	lineBasedFile.C\
	MOLFile.C\
	SDFile.C\
	MOL2File.C\
	NMRStarFile.C\
	parameters.C\
	parameterSection.C\
	PDBdefs.C\
	PDBFileGeneral.C\
	PDBFileDetails.C\
	PDBInfo.C\
	PDBRecords.C\
	resourceFile.C\
	trajectoryFile.C\
	TRRFile.C\
	XYZFile.C

include ../common.mak
-include .Dependencies

depend: $(PARSER_SOURCES)

default_clean:	clean_flex_bison

clean_flex_bison:
	@-$(RM) GAMESSDatParserParser.C GAMESSDatParserLexer.C GAMESSDatParserParser.h
	@-$(RM) GAMESSLogParserParser.C GAMESSLogParserLexer.C GAMESSLogParserParser.h

GAMESSDatParserParser.C: GAMESSDatParserParser.y
	$(YACC) -p GAMESSDatParser -d $*.y && $(MV) y.tab.h $*.h && $(MV) y.tab.c $@
GAMESSDatParserParser.h: GAMESSDatParserParser.y


GAMESSDatParserLexer.C: GAMESSDatParserLexer.l
	$(LEX) -PGAMESSDatParser -o$@ $*.l


GAMESSDatParserLexer.o: GAMESSDatParserLexer.C GAMESSDatParserParser.C
	$(CXX) $(CXXFLAGS) $(ADD_CXXFLAGS) $(CPP_MODE_FLAGS) $(LIB_CXXFLAGS) $(BALL_INCLUDES) $(ADD_INCLUDES) -c $*.C -o $@

GAMESSDatParserParser.o: GAMESSDatParserParser.C
	$(CXX) $(CXXFLAGS) $(ADD_CXXFLAGS) $(CPP_MODE_FLAGS) $(LIB_CXXFLAGS) $(BALL_INCLUDES) $(ADD_INCLUDES) -c $*.C -o $@

PARSER_PREFIX=GAMESSLogParser

GAMESSLogParserParser.C: GAMESSLogParserParser.y
GAMESSLogParserParser.h: GAMESSLogParserParser.y

GAMESSLogParserLexer.o: GAMESSLogParserLexer.C GAMESSLogParserParser.C
	$(CXX) $(CXXFLAGS) $(ADD_CXXFLAGS) $(CPP_MODE_FLAGS) $(LIB_CXXFLAGS) $(BALL_INCLUDES) $(ADD_INCLUDES) -c $*.C -o $@

GAMESSLogParserParser.o: GAMESSLogParserParser.C
	$(CXX) $(CXXFLAGS) $(ADD_CXXFLAGS) $(CPP_MODE_FLAGS) $(LIB_CXXFLAGS) $(BALL_INCLUDES) $(ADD_INCLUDES) -c $*.C -o $@

# TEST
gamess: gamess.o GAMESSDatFile.o GAMESSDatParserLexer.o GAMESSDatParserParser.o GAMESSLogFile.o GAMESSLogParserLexer.o GAMESSLogParserParser.o
	$(CXX)	$(LDFLAGS) -o $@ $@.o $(LIBS) GAMESSLogFile.o GAMESSLogParserLexer.o GAMESSLogParserParser.o 
