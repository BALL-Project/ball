### list all filenames of the directory here ###
SET(SOURCES_LIST
	modularWidgetPlugin.C
	modularWidgetPluginHandler.C
	VIEWPlugin.C
	inputDevPluginHandler.C
)

### the list of all files requiring a moc run ###
SET(MOC_SOURCES_LIST "")

ADD_VIEW_SOURCES("VIEW/PLUGIN" "${SOURCES_LIST}")

ADD_BALL_MOCFILES("VIEW/PLUGIN" "include/BALL/VIEW/PLUGIN" "${MOC_SOURCES_LIST}")


