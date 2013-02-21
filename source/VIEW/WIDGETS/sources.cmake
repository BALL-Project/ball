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
#	canvasWidget.C
#	regularData2DWidget.C
#	regularData1DWidget.C
)

IF(USE_QTWEBKIT)
	LIST(APPEND SOURCES_LIST
		HTMLView.C
	)
ENDIF()

IF(BALL_PYTHON_SUPPORT)
	SET(SOURCES_LIST ${SOURCES_LIST} pyWidget.C hotkeyTable.C)
ENDIF()

### the list of all files requiring a moc run ###
SET(MOC_SOURCES_LIST "${SOURCES_LIST}")

ADD_VIEW_SOURCES("VIEW/WIDGETS" "${SOURCES_LIST}")

ADD_BALL_MOCFILES("VIEW/WIDGETS" "include/BALL/VIEW/WIDGETS" "${MOC_SOURCES_LIST}")
