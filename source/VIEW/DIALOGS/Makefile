#
# $Id: Makefile,v 1.6 2003/08/27 08:59:36 amoll Exp $
#

DIRECTORY=VIEW/DIALOGS

# Where the headers of the derived classes reside
HEADER_DIR		=$(BALL_PATH)/include/BALL/$(DIRECTORY)

# Where the UIC-created headers reside
UIC_DIR				=$(BALL_PATH)/include/BALL/VIEW/UIC

THISLIB=$(LIBNAME)

CPP_SOURCES=\
	amberMinimizationDialog.C\
	atomProperties.C\
	fileDialog.C\
	preferences.C\
	colorMeshDialog.C\
	colorMeshDialogData.C\
	contourSurfaceDialog.C\
	contourSurfaceDialogData.C\
	displayProperties.C\
	displayPropertiesData.C\
	dlgMoveOverlay.C\
	dlgMoveOverlayData.C\
	labelProperties.C\
	labelPropertiesData.C\
	molecularDynamicsDialog.C\
	molecularDynamicsDialogData.C\
	snapShotVisualisation.C\
	snapShotVisualisationData.C\
	molecularFileDialog.C\
	mouseSensitivity.C\
	mouseSensitivityData.C\
	setCamera.C\
	setCameraData.C\
	lightSettings.C\
	lightSettingsData.C\
	stageSettings.C\
	stageSettingsData.C\
	pythonSettings.C\
	pythonSettingsData.C\
	FDPBDialog.C\
	FDPBDialogData.C\
	transformationDialog.C\
	transformationDialogData.C\
	parsedFunctionDialog.C\
	parsedFunctionDialogData.C

MOC_SOURCES=\
	amberMinimizationDialog_moc.C\
	atomProperties_moc.C\
	contourSurfaceDialog_moc.C\
	fileDialog_moc.C\
	preferences_moc.C\
	colorMeshDialog_moc.C\
	colorMeshDialogData_moc.C\
	displayProperties_moc.C\
	displayPropertiesData_moc.C\
	dlgMoveOverlay_moc.C\
	dlgMoveOverlayData_moc.C\
	labelProperties_moc.C\
	labelPropertiesData_moc.C\
	molecularFileDialog_moc.C\
	molecularDynamicsDialog_moc.C\
	molecularDynamicsDialogData_moc.C\
	snapShotVisualisation_moc.C\
	snapShotVisualisationData_moc.C\
	mouseSensitivity_moc.C\
	mouseSensitivityData_moc.C\
	setCamera_moc.C\
	setCameraData_moc.C\
	lightSettings_moc.C\
	lightSettingsData_moc.C\
	stageSettings_moc.C\
	stageSettingsData_moc.C\
	pythonSettings_moc.C\
	pythonSettingsData_moc.C\
	FDPBDialog_moc.C\
	FDPBDialogData_moc.C\
	transformationDialog_moc.C\
	transformationDialogData_moc.C\
	parsedFunctionDialog_moc.C\
	parsedFunctionDialogData_moc.C

UIC_SOURCES=\
	amberMinimizationDialogData.C\
	atomPropertiesData.C\
	colorMeshDialogData.C\
	contourSurfaceDialogData.C\
	displayPropertiesData.C\
	molecularDynamicsDialogData.C\
	snapShotVisualisationData.C\
	mouseSensitivityData.C\
	setCameraData.C\
	lightSettingsData.C\
	stageSettingsData.C\
	pythonSettingsData.C\
	FDPBDialogData.C\
	transformationDialogData.C\
	parsedFunctionDialogData.C

UIC_HDR=$(UIC_SOURCES:Data.C=.dh)

include ../../common.mak
include .Dependencies

BALL_INCLUDES=$(VIEW_INCLUDES)

$(UIC_SOURCES):	$(UIC_HDR)

depend: $(UIC_SOURCES) $(MOC_SOURCES)

amberMinimizationDialog_moc.C:	$(HEADER_DIR)/amberMinimizationDialog.h
	$(MOC) $(HEADER_DIR)/amberMinimizationDialog.h -o $@

atomProperties_moc.C:	$(HEADER_DIR)/atomProperties.h
	$(MOC) $(HEADER_DIR)/atomProperties.h -o $@

contourSurfaceDialog_moc.C:	$(HEADER_DIR)/contourSurfaceDialog.h
	$(MOC) $(HEADER_DIR)/contourSurfaceDialog.h -o $@

displayProperties_moc.C:	$(HEADER_DIR)/displayProperties.h
	$(MOC) $(HEADER_DIR)/displayProperties.h -o $@

fileDialog_moc.C:	$(HEADER_DIR)/fileDialog.h
	$(MOC) $(HEADER_DIR)/fileDialog.h -o $@

labelProperties_moc.C:	$(HEADER_DIR)/labelProperties.h
	$(MOC) $(HEADER_DIR)/labelProperties.h -o $@

preferences_moc.C:	$(HEADER_DIR)/preferences.h
	$(MOC) $(HEADER_DIR)/preferences.h -o $@

dlgMoveOverlay_moc.C:	$(HEADER_DIR)/dlgMoveOverlay.h
	$(MOC) $(HEADER_DIR)/dlgMoveOverlay.h -o $@

dlgMoveOverlayData_moc.C:	$(HEADER_DIR)/dlgMoveOverlayData.h
	$(MOC) $(HEADER_DIR)/dlgMoveOverlayData.h -o $@

molecularDynamicsDialog_moc.C:	$(HEADER_DIR)/molecularDynamicsDialog.h
	$(MOC) $(HEADER_DIR)/molecularDynamicsDialog.h -o $@

snapShotVisualisation_moc.C:	$(HEADER_DIR)/snapShotVisualisation.h
	$(MOC) $(HEADER_DIR)/snapShotVisualisation.h -o $@
