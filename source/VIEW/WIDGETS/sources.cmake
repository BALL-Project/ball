### The ui files go here ###
SET(UI_LIST
	HTMLViewErrorWidget.ui
)

### list all filenames of the directory here ###
SET(SOURCES_LIST
	colorButton.C
	colorTable.C
	datasetControl.C
	dockingController.C
	dockWidget.C
	fileObserver.C
 	genericControl.C
	geometricControl.C
	gridColorWidget.C
	logView.C
	molecularControl.C
	molecularStructure.C
	propertyEditor.C
	scene.C
	shortcutTableView.C
	SDWidget.C
	testFramework.C
	textEditorWidget.C
)

IF(BALL_HAS_QTWEBENGINE)
	LIST(APPEND SOURCES_LIST
		HTMLPage.C
		HTMLView.C
	)
ENDIF()

ADD_VIEW_SOURCES("VIEW/WIDGETS" "${SOURCES_LIST}")
ADD_BALL_UIFILES("VIEW/WIDGETS" "${UI_LIST}")
