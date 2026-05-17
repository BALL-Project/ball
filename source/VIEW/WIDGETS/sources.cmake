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
	inspector/inspectorEmptyState.C
	inspector/sections/stageSection.C
	# Phase 999.44 Plan 03 — Selection tab.
	inspector/selectionAdapter.C
	inspector/sections/selectionSummarySection.C
	inspector/sections/propertiesSection.C
	inspector/sections/quickActionsSection.C
	# Phase 999.44 Plan 04 — Representation tab (3 sections live,
	# Label + Clipping deferred to 999.44-04b RC patch).
	inspector/representationAdapter.C
	inspector/sections/repHeaderSection.C
	inspector/sections/modelSection.C
	inspector/sections/coloringSection.C
	inspector/sections/materialSection.C
	# Phase 999.44 Plan 05 — Scene tab remainder.
	inspector/sections/cameraSection.C
	inspector/sections/lightsSection.C
	inspector/sections/stereoSection.C
	inspector/sections/backgroundSection.C
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
	# Phase 999.46 (Handover §6.3) — CommandPalette. Whole TU gated
	# behind #ifdef BALL_UI_V2; OFF cell produces an empty TU.
	commandPalette.C
	# Phase 999.47 (Handover §7.1) — WelcomeScreen. Whole TU gated
	# behind #ifdef BALL_UI_V2; OFF cell produces an empty TU (same
	# pattern as commandPalette.C — see CMakeLists.txt note about
	# the BALL_UI_V2 propagation fix at 62f52fa2cf).
	welcomeScreen.C
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
