### The ui files go here ###
SET(UI_LIST
	assignBondOrderConfigurationDialog.ui
	assignBondOrderResultsDialog.ui
	atomOverview.ui
	amberConfigurationDialog.ui
	bondProperties.ui
	charmmConfigurationDialog.ui
	contourSurfaceDialog.ui
	coloringSettingsDialog.ui
	compositeProperties.ui
	clippingDialog.ui
	dockDialog.ui
	dockProgressDialog.ui
	dockResultDialog.ui
	downloadPDBFile.ui
	downloadElectronDensity.ui
	editSettings.ui
	editSingleShortcut.ui
	editorPDBInfoDialog.ui
	displayProperties.ui
	fieldLinesDialog.ui
	FDPBDialog.ui
	generateCrystalDialog.ui
	gridColorWidget.ui
	gridVisualizationDialog.ui
	labelDialog.ui
	lightSettings.ui
	mainControlPreferences.ui
	materialSettings.ui
	minimizationDialog.ui
	modelSettingsDialog.ui
	modifyRepresentationDialog.ui
	molecularDynamicsDialog.ui
	MMFF94ConfigurationDialog.ui
	networkPreferences.ui
	openSavePreferences.ui
	parsedFunctionDialog.ui
	peptideDialog.ui
	pluginDialog.ui
	preferences.ui
	propEditorWidget.ui
	propertyEditor.ui
	exportGeometryDialog.ui
	PTEDialog.ui
	pubchemDialog.ui
	raytraceableContourSurfaceDialog.ui
	setCamera.ui
	setClippingPlane.ui
	shortcutDialog.ui
	snapShotVisualisation.ui
	stageSettings.ui
	stereoSettingsDialog.ui
	undoManagerDialog.ui
)

### list all filenames of the directory here ###
SET(SOURCES_LIST
	assignBondOrderConfigurationDialog.C
	assignBondOrderResultsDialog.C
	atomOverview.C
	amberConfigurationDialog.C
	bondProperties.C
	charmmConfigurationDialog.C
	contourSurfaceDialog.C
	coloringSettingsDialog.C
	compositeProperties.C
	clippingDialog.C
	dockDialog.C
	dockProgressDialog.C
	dockResultDialog.C
	downloadPDBFile.C
	downloadElectronDensity.C
	editSettings.C
	editSingleShortcut.C
	displayProperties.C
	fieldLinesDialog.C
	FDPBDialog.C
	generateCrystalDialog.C
	gridVisualizationDialog.C
	labelDialog.C
	lightSettings.C
	mainControlPreferences.C
	materialSettings.C
	minimizationDialog.C
	modelSettingsDialog.C
	modifyRepresentationDialog.C
	molecularDynamicsDialog.C
	molecularFileDialog.C
	MMFF94ConfigurationDialog.C
	networkPreferences.C
	openSavePreferences.C
	parsedFunctionDialog.C
	peptideDialog.C
	pluginDialog.C
	preferences.C
	exportGeometryDialog.C
	PTEDialog.C
	pubchemDialog.C
	raytraceableContourSurfaceDialog.C
	setCamera.C
	setClippingPlane.C
	serverPreferences.C
	shortcutDialog.C
	snapShotVisualisation.C
	stageSettings.C
	stereoSettingsDialog.C
	undoManagerDialog.C
)

IF(BALL_PYTHON_SUPPORT)
	SET(UI_LIST ${UI_LIST} pythonSettings.ui)
	SET(SOURCES_LIST ${SOURCES_LIST} pythonSettings.C)
ENDIF()

IF(BALL_HAS_FFTW)
	SET(UI_LIST ${UI_LIST} geometricFitDialog.ui)
	SET(SOURCES_LIST ${SOURCES_LIST} geometricFitDialog.C)
ENDIF()

IF(BALL_HAS_QTWEBENGINE)
	SET(UI_LIST ${UI_LIST} webEnginePreferences.ui)
	SET(SOURCES_LIST ${SOURCES_LIST} webEnginePreferences.C)
ENDIF()

ADD_VIEW_SOURCES("VIEW/DIALOGS" "${SOURCES_LIST}")

ADD_BALL_UIFILES("VIEW/DIALOGS" "${UI_LIST}")
