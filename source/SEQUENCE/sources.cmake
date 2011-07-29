### list all filenames of the directory here ###
SET(SOURCES_LIST
	alignment.C
	sequence.C
	sequenceCharacter.C
	sequenceIterator.C
)

ADD_BALL_SOURCES("SEQUENCE" "${SOURCES_LIST}")
