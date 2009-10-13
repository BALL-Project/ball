### list all filenames of the directory here ###
SET(SOURCES_LIST
	demoTutorialDialog.C
	icons.C
	main.C
	mainframe.C
)

### the list of all files requiring a moc run ###
SET(MOC_SOURCES_LIST
	demoTutorialDialog.C
	mainframe.C
)

QT4_ADD_RESOURCES("source/APPLICATIONS/BALLVIEW/splashscreen.rcc")
ADD_VIEW_SOURCES("VIEW/WIDGETS" "${SOURCES_LIST}")

ADD_BALL_MOCFILES("VIEW/WIDGETS" "include/BALL/VIEW/WIDGETS" "${MOC_SOURCES_LIST}")

