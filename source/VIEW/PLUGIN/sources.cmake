### list all filenames of the directory here ###
SET(SOURCES_LIST
	modularWidgetPlugin.C
	modularWidgetPluginHandler.C
	VIEWPlugin.C
	inputDevPluginHandler.C
)

ADD_VIEW_SOURCES("VIEW/PLUGIN" "${SOURCES_LIST}")
