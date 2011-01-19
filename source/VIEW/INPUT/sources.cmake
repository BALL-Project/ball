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

### the list of all files requiring a moc run ###
SET(MOC_SOURCES_LIST inputPlugin.C)

ADD_VIEW_SOURCES("VIEW/INPUT" "${SOURCES_LIST}")

ADD_BALL_MOCFILES("VIEW/INPUT" "include/BALL/VIEW/INPUT" "${MOC_SOURCES_LIST}")
