DIRECTORY=KERNEL

THISLIB=$(LIBNAME)

PARSER_SOURCES= \
	expressionParser.C\
	expressionParserParser.C\
	expressionParserLexer.C

CPP_SOURCES= \
	atom.C\
	atomContainer.C\
	bond.C\
	chain.C\
	expression.C\
	expressionPredicate.C\
	expressionTree.C\
	expressionParser.C\
	expressionParserLexer.C\
	expressionParserParser.C\
	fragment.C\
	global.C\
	molecule.C\
	nucleicAcid.C\
	nucleotide.C\
	PDBAtom.C\
	protein.C\
	PTE.C\
	residue.C\
	secondaryStructure.C\
	selector.C\
	standardPredicates.C\
	system.C


#
# the prefix for the methods defined in the lexer
#
PARSER_PREFIX=ExpressionParser

include ../common.mak
include .Dependencies

depend: $(PARSER_SOURCES)

default_clean:	clean_flex_bison

clean_flex_bison:
	@-$(RM) expressionParserParser.C expressionParserLexer.C expressionParserParser.h

expressionParserParser.C: expressionParserParser.y
expressionParserParser.h: expressionParserParser.y

expressionParserLexer.o: expressionParserLexer.C expressionParserParser.C
	$(CXX) $(CXXFLAGS) $(ADD_CXXFLAGS) $(CPP_MODE_FLAGS) $(LIB_CXXFLAGS) $(BALL_INCLUDES) $(ADD_INCLUDES) -c $*.C -o $@

expressionParserParser.o: expressionParserParser.C
	$(CXX) $(CXXFLAGS) $(ADD_CXXFLAGS) $(CPP_MODE_FLAGS) $(LIB_CXXFLAGS) $(BALL_INCLUDES) $(ADD_INCLUDES) -c $*.C -o $@
