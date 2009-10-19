### the directory name ###
SET(DIRECTORY source/CONCEPT)

### list all filenames of the directory here ###
SET(SOURCES_LIST
	autoDeletable.C
	composite.C
	embeddable.C
	enumerator.C
	factory.C
	object.C
	objectCreator.C
	moleculeObjectCreator.C
	persistenceManager.C
	preferencesObject.C
	processor.C
	property.C
	selectable.C
	textPersistenceManager.C
	timeStamp.C
	visitor.C
	molecularInformation.C
)

IF(BALL_HAS_XDR)
	LIST(APPEND SOURCES_LIST XDRPersistenceManager.C)
ENDIF()

### asio dependent sources ###
IF (BALL_HAS_ASIO)
	SET(SOURCES_LIST ${SOURCES_LIST} client.C)
ENDIF()

ADD_BALL_SOURCES("CONCEPT" "${SOURCES_LIST}")
