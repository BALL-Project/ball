### the directory name ###
SET(DIRECTORY source/KERNEL)

### list all filenames of the directory here ###
SET(SOURCES_LIST
	atom.C
	atomContainer.C
	bond.C
	chain.C
	expression.C
	expressionPredicate.C
	expressionTree.C
	expressionParser.C
	extractors.C
	fragment.C
	global.C
	molecularInteractions.C
	molecule.C
	nucleicAcid.C
	nucleotide.C
	PDBAtom.C
	predicate.C
	protein.C
	PTE.C
	residue.C
	secondaryStructure.C
	selector.C
	standardPredicates.C
	system.C
#	extractors.C\
)	

ADD_BALL_SOURCES("KERNEL" "${SOURCES_LIST}")

ADD_BALL_PARSER_LEXER("KERNEL" "expressionParser" "ExpressionParser")
