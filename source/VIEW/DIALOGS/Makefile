#
# $Id: Makefile,v 1.31 2004/02/02 18:57:51 anhi Exp $
#

DIRECTORY=VIEW/DIALOGS

# Where the headers of the derived classes reside
HEADER_DIR=$(BALL_PATH)/include/BALL/$(DIRECTORY)

# Where the UIC-created headers reside
UIC_DIR=$(BALL_PATH)/include/BALL/VIEW/UIC

THISLIB=$(LIBNAME)

CPP_SOURCES=\
	amberConfigurationDialog.C\
	amberConfigurationDialogData.C\
	amberMinimizationDialog.C\
	amberMinimizationDialogData.C\
	atomProperties.C\
	atomPropertiesData.C\
	bondProperties.C\
	bondPropertiesData.C\
	colorMeshDialog.C\
	colorMeshDialogData.C\
	coloringSettingsDialog.C\
	coloringSettingsDialogData.C\
	contourSurfaceDialog.C\
	contourSurfaceDialogData.C\
	downloadPDBFile.C\
	downloadPDBFileData.C\
	displayProperties.C\
	displayPropertiesData.C\
	FDPBDialog.C\
	FDPBDialogData.C\
	labelDialog.C\
	labelDialogData.C\
	lightSettings.C\
	lightSettingsData.C\
	mainControlPreferences.C\
	mainControlPreferencesData.C\
	modelSettingsDialog.C\
	modelSettingsDialogData.C\
	molecularDynamicsDialog.C\
	molecularDynamicsDialogData.C\
	molecularFileDialog.C\
	mouseSensitivity.C\
	mouseSensitivityData.C\
	openBruker1DFile.C\
	parsedFunctionDialog.C\
	parsedFunctionDialogData.C\
	peptideDialog.C\
	peptideDialogData.C\
	preferences.C\
	preferencesData.C\
	setCamera.C\
	setCameraData.C\
	pythonSettings.C\
	pythonSettingsData.C\
	selectorDialog.C\
	selectorDialogData.C\
	serverPreferences.C\
	snapShotVisualisation.C\
	snapShotVisualisationData.C\
	stageSettings.C\
	stageSettingsData.C\
	transformationDialog.C\
	transformationDialogData.C

MOC_SOURCES=\
	amberConfigurationDialog_moc.C\
	amberConfigurationDialogData_moc.C\
	amberMinimizationDialog_moc.C\
	amberMinimizationDialogData_moc.C\
	atomProperties_moc.C\
	atomPropertiesData_moc.C\
	bondProperties_moc.C\
	bondPropertiesData_moc.C\
	coloringSettingsDialog_moc.C\
	coloringSettingsDialogData_moc.C\
	colorMeshDialog_moc.C\
	colorMeshDialogData_moc.C\
	contourSurfaceDialog_moc.C\
	contourSurfaceDialogData_moc.C\
	downloadPDBFile_moc.C\
	downloadPDBFileData_moc.C\
	displayProperties_moc.C\
	displayPropertiesData_moc.C\
	FDPBDialog_moc.C\
	FDPBDialogData_moc.C\
	labelDialog_moc.C\
	labelDialogData_moc.C\
	lightSettings_moc.C\
	lightSettingsData_moc.C\
	mainControlPreferences_moc.C\
	mainControlPreferencesData_moc.C\
	modelSettingsDialog_moc.C\
	modelSettingsDialogData_moc.C\
	molecularFileDialog_moc.C\
	molecularDynamicsDialog_moc.C\
	molecularDynamicsDialogData_moc.C\
	mouseSensitivity_moc.C\
	mouseSensitivityData_moc.C\
	openBruker1DFile_moc.C\
	parsedFunctionDialog_moc.C\
	parsedFunctionDialogData_moc.C\
	peptideDialog_moc.C\
	peptideDialogData_moc.C\
	preferences_moc.C\
	preferencesData_moc.C\
	pythonSettings_moc.C\
	pythonSettingsData_moc.C\
	selectorDialog_moc.C\
	selectorDialogData_moc.C\
	serverPreferences_moc.C\
	setCamera_moc.C\
	setCameraData_moc.C\
	snapShotVisualisation_moc.C\
	snapShotVisualisationData_moc.C\
	stageSettings_moc.C\
	stageSettingsData_moc.C\
	transformationDialog_moc.C\
	transformationDialogData_moc.C

UIC_SOURCES=\
	amberConfigurationDialogData.C\
	amberMinimizationDialogData.C\
	atomPropertiesData.C\
	bondPropertiesData.C\
	coloringSettingsDialogData.C\
	colorMeshDialogData.C\
	contourSurfaceDialogData.C\
	downloadPDBFileData.C\
	displayPropertiesData.C\
	FDPBDialogData.C\
	labelDialogData.C\
	lightSettingsData.C\
	mainControlPreferencesData.C\
	modelSettingsDialogData.C\
	molecularDynamicsDialogData.C\
	mouseSensitivityData.C\
	parsedFunctionDialogData.C\
	peptideDialogData.C\
	preferencesData.C\
	pythonSettingsData.C\
	selectorDialogData.C\
	setCameraData.C\
	snapShotVisualisationData.C\
	stageSettingsData.C\
	transformationDialogData.C

UIC_HDR=$(UIC_SOURCES:Data.C=.dh)

include ../../common.mak
include .Dependencies

BALL_INCLUDES=$(VIEW_INCLUDES)

molecularFileDialog_moc.C: $(HEADER_DIR)/molecularFileDialog.h
	$(MOC) $(HEADER_DIR)/molecularFileDialog.h -o $@

serverPreferences_moc.C: $(HEADER_DIR)/serverPreferences.h
	$(MOC) $(HEADER_DIR)/serverPreferences.h -o $@

openBruker1DFile_moc.C: $(HEADER_DIR)/openBruker1DFile.h
	$(MOC) $(HEADER_DIR)/openBruker1DFile.h -o $@

depend: $(UIC_SOURCES) $(MOC_SOURCES)

default_clean: clean_uic

clean_uic:
	@echo "Cleaning UIC files in $(UIC_DIR)..."
	@$(RM) -f $(UIC_DIR)/*.h
