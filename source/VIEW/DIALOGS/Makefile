#
# $Id: Makefile,v 1.22 2003/11/19 00:46:28 amoll Exp $
#

DIRECTORY=VIEW/DIALOGS

# Where the headers of the derived classes reside
HEADER_DIR=$(BALL_PATH)/include/BALL/$(DIRECTORY)

# Where the UIC-created headers reside
UIC_DIR=$(BALL_PATH)/include/BALL/VIEW/UIC

THISLIB=$(LIBNAME)

CPP_SOURCES=\
	amberMinimizationDialog.C\
	amberMinimizationDialogData.C\
	atomProperties.C\
	atomPropertiesData.C\
	colorMeshDialog.C\
	colorMeshDialogData.C\
	coloringSettingsDialog.C\
	coloringSettingsDialogData.C\
	contourSurfaceDialog.C\
	contourSurfaceDialogData.C\
	displayProperties.C\
	displayPropertiesData.C\
	moveOverlayDialog.C\
	moveOverlayDialogData.C\
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
	openBruker2DFile.C\
	parsedFunctionDialog.C\
	parsedFunctionDialogData.C\
	peptideDialog.C\
	peptideDialogData.C\
	preferences.C\
	setCamera.C\
	setCameraData.C\
	pythonSettings.C\
	pythonSettingsData.C\
	serverPreferences.C\
	snapShotVisualisation.C\
	snapShotVisualisationData.C\
	stageSettings.C\
	stageSettingsData.C\
	transformationDialog.C\
	transformationDialogData.C

MOC_SOURCES=\
	amberMinimizationDialog_moc.C\
	amberMinimizationDialogData_moc.C\
	atomProperties_moc.C\
	atomPropertiesData_moc.C\
	coloringSettingsDialog_moc.C\
	coloringSettingsDialogData_moc.C\
	colorMeshDialog_moc.C\
	colorMeshDialogData_moc.C\
	contourSurfaceDialog_moc.C\
	contourSurfaceDialogData_moc.C\
	displayProperties_moc.C\
	displayPropertiesData_moc.C\
	moveOverlayDialog_moc.C\
	moveOverlayDialogData_moc.C\
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
	openBruker2DFile_moc.C\
	parsedFunctionDialog_moc.C\
	parsedFunctionDialogData_moc.C\
	peptideDialog_moc.C\
	peptideDialogData_moc.C\
	preferences_moc.C\
	pythonSettings_moc.C\
	pythonSettingsData_moc.C\
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
	amberMinimizationDialogData.C\
	atomPropertiesData.C\
	coloringSettingsDialogData.C\
	colorMeshDialogData.C\
	contourSurfaceDialogData.C\
	displayPropertiesData.C\
	FDPBDialogData.C\
	labelDialogData.C\
	lightSettingsData.C\
	mainControlPreferencesData.C\
	modelSettingsDialogData.C\
	molecularDynamicsDialogData.C\
	moveOverlayDialogData.C\
	mouseSensitivityData.C\
	parsedFunctionDialogData.C\
	peptideDialogData.C\
	pythonSettingsData.C\
	setCameraData.C\
	snapShotVisualisationData.C\
	stageSettingsData.C\
	transformationDialogData.C

UIC_HDR=$(UIC_SOURCES:Data.C=.dh)

include ../../common.mak
include .Dependencies

BALL_INCLUDES=$(VIEW_INCLUDES)

$(UIC_SOURCES):	$(UIC_HDR)

molecularFileDialog_moc.C: $(HEADER_DIR)/molecularFileDialog.h
	$(MOC) $(HEADER_DIR)/molecularFileDialog.h -o $@

serverPreferences_moc.C: $(HEADER_DIR)/serverPreferences.h
	$(MOC) $(HEADER_DIR)/serverPreferences.h -o $@

openBruker1DFile_moc.C: $(HEADER_DIR)/openBruker1DFile.h
	$(MOC) $(HEADER_DIR)/openBruker1DFile.h -o $@

openBruker2DFile_moc.C: $(HEADER_DIR)/openBruker2DFile.h
	$(MOC) $(HEADER_DIR)/openBruker2DFile.h -o $@

preferences_moc.C: $(HEADER_DIR)/preferences.h
	$(MOC) $(HEADER_DIR)/preferences.h -o $@

depend: $(UIC_SOURCES) $(MOC_SOURCES)

default_clean: clean_uic

clean_uic:
	@echo "Cleaning UIC files in $(UIC_DIR)..."
	@$(RM) -f $(UIC_DIR)/*.h
