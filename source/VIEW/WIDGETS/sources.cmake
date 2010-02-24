### list all filenames of the directory here ###
SET(SOURCES_LIST
	colorTable.C
	datasetControl.C
	dockingController.C
	dockWidget.C
	editableScene.C
	fileObserver.C
 	genericControl.C
	geometricControl.C
	helpViewer.C
	hotkeyTable.C
	logView.C
	molecularControl.C
	molecularStructure.C
	scene.C
	shortcutTableView.C
	SDWidget.C
	testFramework.C
#	canvasWidget.C
#	regularData2DWidget.C
#	regularData1DWidget.C
)

IF(BALL_PYTHON_SUPPORT)
	SET(SOURCES_LIST ${SOURCES_LIST} pyWidget.C)
ENDIF()

### the list of all files requiring a moc run ###
SET(MOC_SOURCES_LIST "${SOURCES_LIST}")

ADD_VIEW_SOURCES("VIEW/WIDGETS" "${SOURCES_LIST}")

ADD_BALL_MOCFILES("VIEW/WIDGETS" "include/BALL/VIEW/WIDGETS" "${MOC_SOURCES_LIST}")
