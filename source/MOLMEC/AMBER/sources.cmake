### the directory name ###
SET(DIRECTORY source/MOLMEC/AMBER)

### list all filenames of the directory here ###
SET(SOURCES_LIST
	amber.C
	amberBend.C
	amberNonBonded.C
	amberStretch.C
	amberTorsion.C
	GAFFTypeProcessor.C
	GAFFCESParser.C
)	

ADD_BALL_SOURCES("MOLMEC/AMBER" "${SOURCES_LIST}")

ADD_BALL_PARSER_LEXER("MOLMEC/AMBER" "GAFFCESParser" "GAFFCESParser")
