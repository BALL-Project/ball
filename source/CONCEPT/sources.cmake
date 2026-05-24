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
	preferencesObject.C
	processor.C
	property.C
	selectable.C
	timeStamp.C
	visitor.C
	molecularInformation.C
)

ADD_BALL_SOURCES("CONCEPT" "${SOURCES_LIST}")
