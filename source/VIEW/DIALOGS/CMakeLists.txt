set(path VIEW/DIALOGS)


SET(UIC_SOURCES "")
SET(UIC_SOURCES 
	amberConfigurationDialog.ui
	assignBondOrderConfigurationDialog.ui
	assignBondOrderResultsDialog.ui
	atomOverview.ui
	bondProperties.ui
	charmmConfigurationDialog.ui
	clippingDialog.ui
	coloringSettingsDialog.ui
	compositeProperties.ui
	contourSurfaceDialog.ui
	displayProperties.ui
	dockDialog.ui
	dockProgressDialog.ui
	dockResultDialog.ui
	downloadPDBFile.ui
	editOperationDialog.ui
	editSettings.ui
	editSingleShortcut.ui
	exportGeometryDialog.ui
	FDPBDialog.ui
	fieldLinesDialog.ui
	geometricFitDialog.ui
	gridVisualizationDialog.ui
	labelDialog.ui
	lightSettings.ui
	mainControlPreferences.ui
	materialSettings.ui
	minimizationDialog.ui
	MMFF94ConfigurationDialog.ui
	modelSettingsDialog.ui
	modifyRepresentationDialog.ui
	molecularDynamicsDialog.ui
	networkPreferences.ui
	openSavePreferences.ui
	parsedFunctionDialog.ui
	peptideDialog.ui
	pluginDialog.ui
	preferences.ui
	PTEDialog.ui
	pubchemDialog.ui
	pythonSettings.ui
	raytraceableContourSurfaceDialog.ui
	runGAMESSDialog.ui
	setCamera.ui
	setClippingPlane.ui
	shortcutDialog.ui
	snapShotVisualisation.ui
	stageSettings.ui
	stereoSettingsDialog.ui
)

foreach(i ${UIC_SOURCES})
	wrap_UIC(VIEW ${path} ${i})
endforeach(i)


SET(MOC_SOURCES "")
SET(MOC_SOURCES 
	amberConfigurationDialog.h
	assignBondOrderConfigurationDialog.h
	assignBondOrderResultsDialog.h
	atomOverview.h
	bondProperties.h
	charmmConfigurationDialog.h
	clippingDialog.h
	coloringSettingsDialog.h
	compositeProperties.h
	contourSurfaceDialog.h
	displayProperties.h
	dockDialog.h
	dockProgressDialog.h
	dockResultDialog.h
	downloadPDBFile.h
	editSettings.h
	editSingleShortcut.h
	exportGeometryDialog.h
	FDPBDialog.h
	fieldLinesDialog.h
	gridVisualizationDialog.h
	labelDialog.h
	lightSettings.h
	mainControlPreferences.h
	materialSettings.h
	minimizationDialog.h
	MMFF94ConfigurationDialog.h
	modelSettingsDialog.h
	modifyRepresentationDialog.h
	molecularDynamicsDialog.h
	molecularFileDialog.h
	networkPreferences.h
	openSavePreferences.h
	parsedFunctionDialog.h
	peptideDialog.h
	pluginDialog.h
	preferences.h
	PTEDialog.h
	pubchemDialog.h
	pythonSettings.h
	raytraceableContourSurfaceDialog.h
	serverPreferences.h
	setCamera.h
	setClippingPlane.h
	shortcutDialog.h
	snapShotVisualisation.h
	stageSettings.h
	stereoSettingsDialog.h
)

foreach(i ${MOC_SOURCES})
	wrap_MOC(VIEW ${path} ${i})
endforeach(i)


list(APPEND VIEW_SOURCES 
	VIEW/DIALOGS/amberConfigurationDialog.C
	VIEW/DIALOGS/assignBondOrderConfigurationDialog.C
	VIEW/DIALOGS/assignBondOrderResultsDialog.C
	VIEW/DIALOGS/atomOverview.C
	VIEW/DIALOGS/bondProperties.C
	VIEW/DIALOGS/charmmConfigurationDialog.C
	VIEW/DIALOGS/clippingDialog.C
	VIEW/DIALOGS/coloringSettingsDialog.C
	VIEW/DIALOGS/compositeProperties.C
	VIEW/DIALOGS/contourSurfaceDialog.C
	VIEW/DIALOGS/displayProperties.C
	VIEW/DIALOGS/dockDialog.C
	VIEW/DIALOGS/dockProgressDialog.C
	VIEW/DIALOGS/dockResultDialog.C
	VIEW/DIALOGS/downloadPDBFile.C
	VIEW/DIALOGS/editSettings.C
	VIEW/DIALOGS/editSingleShortcut.C
	VIEW/DIALOGS/exportGeometryDialog.C
	VIEW/DIALOGS/FDPBDialog.C
	VIEW/DIALOGS/fieldLinesDialog.C
	VIEW/DIALOGS/gridVisualizationDialog.C
	VIEW/DIALOGS/labelDialog.C
	VIEW/DIALOGS/lightSettings.C
	VIEW/DIALOGS/mainControlPreferences.C
	VIEW/DIALOGS/materialSettings.C
	VIEW/DIALOGS/minimizationDialog.C
	VIEW/DIALOGS/MMFF94ConfigurationDialog.C
	VIEW/DIALOGS/modelSettingsDialog.C
	VIEW/DIALOGS/modifyRepresentationDialog.C
	VIEW/DIALOGS/molecularDynamicsDialog.C
	VIEW/DIALOGS/molecularFileDialog.C
	VIEW/DIALOGS/networkPreferences.C
	VIEW/DIALOGS/openSavePreferences.C
	VIEW/DIALOGS/parsedFunctionDialog.C
	VIEW/DIALOGS/peptideDialog.C
	VIEW/DIALOGS/pluginDialog.C
	VIEW/DIALOGS/preferences.C
	VIEW/DIALOGS/PTEDialog.C
	VIEW/DIALOGS/pubchemDialog.C
	VIEW/DIALOGS/pythonSettings.C
	VIEW/DIALOGS/raytraceableContourSurfaceDialog.C
	VIEW/DIALOGS/serverPreferences.C
	VIEW/DIALOGS/setCamera.C
	VIEW/DIALOGS/setClippingPlane.C
	VIEW/DIALOGS/shortcutDialog.C
	VIEW/DIALOGS/snapShotVisualisation.C
	VIEW/DIALOGS/stageSettings.C
	VIEW/DIALOGS/stereoSettingsDialog.C
)

