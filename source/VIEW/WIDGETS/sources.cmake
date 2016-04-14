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
	helpViewer.C
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

IF(BALL_HAS_QTWEBKIT)
	LIST(APPEND SOURCES_LIST
		HTMLView.C
	)
ENDIF()

IF(BALL_PYTHON_SUPPORT)
	SET(SOURCES_LIST ${SOURCES_LIST} pyWidget.C hotkeyTable.C)
ENDIF()

ADD_VIEW_SOURCES("VIEW/WIDGETS" "${SOURCES_LIST}")
