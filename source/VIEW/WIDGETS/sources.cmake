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
	helpViewer.C
	logView.C
	molecularControl.C
	molecularStructure.C
	propertyEditor.C
	scene.C
	# Phase 999.43 — shared widgets (load-bearing for 999.44 Inspector).
	formRow.C
	labeledSlider.C
	sectionHeader.C
	swatchButton.C
	# Phase 999.44 — Unified Inspector shell + sections. Each TU is
	# fully gated by #ifdef BALL_UI_V2 in its body so listing them
	# unconditionally keeps the OFF cell's symbol table identical to
	# the pre-999.44 baseline.
	inspector/inspectorSection.C
	inspector/inspectorTabs.C
	inspector/inspectorBody.C
	inspector/inspectorView.C
	inspector/inspectorDock.C
	# Phase 999.45 — Workspace consolidation: ProjectDock (left-rail
	# tabified group of Structures/Reps/Datasets) + BottomDrawer
	# (24px↔240px animated dock with Logs/FileObserver tabs). Both
	# compile in BALL_UI_V2=ON and OFF cells; Mainframe wires them
	# under the ifdef.
	projectDock.C
	bottomDrawer.C
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

IF(BALL_PYTHON_SUPPORT)
	SET(SOURCES_LIST ${SOURCES_LIST} pyWidget.C hotkeyTable.C)
ENDIF()

ADD_VIEW_SOURCES("VIEW/WIDGETS" "${SOURCES_LIST}")
ADD_BALL_UIFILES("VIEW/WIDGETS" "${UI_LIST}")
