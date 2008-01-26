DIRECTORY=MOLMEC/AMBER

THISLIB=$(LIBNAME)

PARSER_SOURCES= \
	GAFFCESParser.C\
	GAFFCESParserParser.C\
	GAFFCESParserLexer.C

CPP_SOURCES=\
	amber.C\
	amberBend.C\
	amberNonBonded.C\
	amberStretch.C\
	amberTorsion.C\
	GAFFTypeProcessor.C\
	GAFFCESParser.C\
	GAFFCESParserParser.C\
	GAFFCESParserLexer.C

include ../../common.mak
-include .Dependencies

depend: $(PARSER_SOURCES)

default_clean:	clean_flex_bison

clean_flex_bison:
	@-$(RM) GAFFCESParserParser.C GAFFCESParserLexer.C GAFFCESParserParser.h 2>/dev/null

GAFFCESParserParser.C: GAFFCESParserParser.y
	$(YACC) -p GAFFCESParser -d $*.y && $(MV) y.tab.h $*.h && $(MV) y.tab.c $@

GAFFCESParserParser.h: GAFFCESParserParser.y

GAFFCESParserLexer.C: GAFFCESParserLexer.l
	$(LEX) -PGAFFCESParser -o$@ $*.l


