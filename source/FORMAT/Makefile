# -*- Mode: C++; tab-width: 2; -*-
# vi: set ts=2:
#
# $Id: Makefile,v 1.25.8.2 2007/04/02 20:54:50 bertsch Exp $
#
# Author:
#   Oliver Kohlbacher
#

DIRECTORY=FORMAT

include ../config.mak

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
	DSN6File.C\
	genericMolFile.C\
	HINFile.C\
	INIFile.C\
	JCAMPFile.C\
	KCFFile.C\
	lineBasedFile.C\
	MOLFile.C\
	SDFile.C\
	MOL2File.C\
	NMRStarFile.C\
	NWChemTRJFile.C\
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
	XYZFile.C\
	SCWRLRotamerFile.C\
	molFileFactory.C

ifeq ($(BALL_HAS_VIEW),true)
	CPP_SOURCES+=DBInterface.C
endif

include ../targets.mak
include ../rules.mak

-include .Dependencies

depend: $(PARSER_SOURCES)

default_clean:	clean_flex_bison

clean_flex_bison:
	@-$(RM) GAMESSDatParserParser.C GAMESSDatParserLexer.C GAMESSDatParserParser.h 2>/dev/null
	@-$(RM) GAMESSLogParserParser.C GAMESSLogParserLexer.C GAMESSLogParserParser.h 2>/dev/null




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
gamess_dat: gamess_dat.o GAMESSDatFile.o GAMESSDatParserLexer.o GAMESSDatParserParser.o GAMESSLogFile.o GAMESSLogParserLexer.o GAMESSLogParserParser.o
	$(CXX)	$(LDFLAGS) -o $@ $@.o $(LIBS) GAMESSDatFile.o GAMESSDatParserLexer.o GAMESSDatParserParser.o 

gamess: gamess.o GAMESSDatFile.o GAMESSDatParserLexer.o GAMESSDatParserParser.o GAMESSLogFile.o GAMESSLogParserLexer.o GAMESSLogParserParser.o
	$(CXX)	$(LDFLAGS) -o $@ $@.o $(LIBS) GAMESSLogFile.o GAMESSLogParserLexer.o GAMESSLogParserParser.o 
