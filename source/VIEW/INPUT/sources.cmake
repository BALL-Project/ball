### list all filenames of the directory here ###
SET(SOURCES_LIST
	deviceEvent.C
	transformationEvent6D.C
	headTrackingEvent.C
	motionTrackingEvent.C
	notificationEvent.C
	buttonEvent.C
	inputDeviceDriver.C
)

ADD_VIEW_SOURCES("VIEW/INPUT" "${SOURCES_LIST}")
