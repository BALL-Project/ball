include ../Make.rules.in

DIRECTORY=KERNEL

_objects_LINK= ../config/genobjects.sh $(DIRECTORY)

BUILT_SOURCES=expressionParserParser.C expressionParserLexer.C expressionParserParser.h

expressionParserParser.C: expressionParserParser.y
expressionParserParser.h: expressionParserParser.y

CURRENTDIR_SOURCES= \
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

clean-local:
	$(RM) -f expressionParserParser.C expressionParserLexer.C expressionParserParser.h
	
#
# the prefix for the methods defined in the lexer
#
PARSER_PREFIX=ExpressionParser
