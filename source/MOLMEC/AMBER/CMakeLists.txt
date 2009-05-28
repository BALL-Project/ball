set(path MOLMEC/AMBER)


SET(YACC_SOURCES "")
SET(YACC_SOURCES 
	GAFFCESParserParser.y
)

foreach(i ${YACC_SOURCES})
	wrap_YACC(BALL ${path} ${i})
endforeach(i)


SET(FLEX_SOURCES "")
SET(FLEX_SOURCES 
	GAFFCESParserLexer.l
)

foreach(i ${FLEX_SOURCES})
	wrap_FLEX(BALL ${path} ${i})
endforeach(i)


list(APPEND BALL_SOURCES 
	MOLMEC/AMBER/amberBend.C
	MOLMEC/AMBER/amber.C
	MOLMEC/AMBER/amberNonBonded.C
	MOLMEC/AMBER/amberStretch.C
	MOLMEC/AMBER/amberTorsion.C
	MOLMEC/AMBER/GAFFCESParser.C
	MOLMEC/AMBER/GAFFTypeProcessor.C
)

