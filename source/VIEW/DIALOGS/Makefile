#
# $Id: Makefile,v 1.38 2004/06/19 14:30:47 amoll Exp $
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
	minimizationDialog.C\
	minimizationDialogData.C\
	bondProperties.C\
	bondPropertiesData.C\
	charmmConfigurationDialog.C\
	charmmConfigurationDialogData.C\
	colorMeshDialog.C\
	colorMeshDialogData.C\
	coloringSettingsDialog.C\
	coloringSettingsDialogData.C\
	compositeProperties.C\
	compositePropertiesData.C\
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
	materialSettings.C\
	materialSettingsData.C\
	modelSettingsDialog.C\
	modelSettingsDialogData.C\
	molecularDynamicsDialog.C\
	molecularDynamicsDialogData.C\
	molecularFileDialog.C\
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
	minimizationDialog_moc.C\
	minimizationDialogData_moc.C\
	compositeProperties_moc.C\
	compositePropertiesData_moc.C\
	bondProperties_moc.C\
	bondPropertiesData_moc.C\
	charmmConfigurationDialog_moc.C\
	charmmConfigurationDialogData_moc.C\
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
	materialSettings_moc.C\
	materialSettingsData_moc.C\
	modelSettingsDialog_moc.C\
	modelSettingsDialogData_moc.C\
	molecularFileDialog_moc.C\
	molecularDynamicsDialog_moc.C\
	molecularDynamicsDialogData_moc.C\
	parsedFunctionDialog_moc.C\
	parsedFunctionDialogData_moc.C\
	peptideDialog_moc.C\
	peptideDialogData_moc.C\
	preferences_moc.C\
	preferencesData_moc.C\
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
	amberConfigurationDialogData.C\
	minimizationDialogData.C\
	bondPropertiesData.C\
	charmmConfigurationDialogData.C\
	coloringSettingsDialogData.C\
	colorMeshDialogData.C\
	compositePropertiesData.C\
	contourSurfaceDialogData.C\
	downloadPDBFileData.C\
	displayPropertiesData.C\
	FDPBDialogData.C\
	labelDialogData.C\
	lightSettingsData.C\
	mainControlPreferencesData.C\
	materialSettingsData.C\
	modelSettingsDialogData.C\
	molecularDynamicsDialogData.C\
	parsedFunctionDialogData.C\
	peptideDialogData.C\
	preferencesData.C\
	pythonSettingsData.C\
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
