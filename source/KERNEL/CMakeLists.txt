set(path KERNEL)


SET(YACC_SOURCES "")
SET(YACC_SOURCES 
	expressionParserParser.y
)

foreach(i ${YACC_SOURCES})
	wrap_YACC(BALL ${path} ${i})
endforeach(i)


SET(FLEX_SOURCES "")
SET(FLEX_SOURCES 
	expressionParserLexer.l
)

foreach(i ${FLEX_SOURCES})
	wrap_FLEX(BALL ${path} ${i})
endforeach(i)


list(APPEND BALL_SOURCES 
	KERNEL/atom.C
	KERNEL/atomContainer.C
	KERNEL/bond.C
	KERNEL/chain.C
	KERNEL/expression.C
	KERNEL/expressionParser.C
	KERNEL/expressionPredicate.C
	KERNEL/expressionTree.C
	KERNEL/extractors.C
	KERNEL/fragment.C
	KERNEL/global.C
	KERNEL/molecule.C
	KERNEL/nucleicAcid.C
	KERNEL/nucleotide.C
	KERNEL/PDBAtom.C
	KERNEL/predicate.C
	KERNEL/protein.C
	KERNEL/PTE.C
	KERNEL/residue.C
	KERNEL/secondaryStructure.C
	KERNEL/selector.C
	KERNEL/standardPredicates.C
	KERNEL/system.C
)

