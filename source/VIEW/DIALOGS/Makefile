# -*- Mode: C++; tab-width: 2; -*-
# vi: set ts=2:
#
# $Id: Makefile,v 1.54.14.2 2007-03-26 07:51:46 amoll Exp $
#
# Author:
#   Oliver Kohlbacher
#

DIRECTORY=VIEW/DIALOGS

include ../../config.mak

# Where the headers of the derived classes reside
HEADER_DIR=$(BALL_PATH)/include/BALL/$(DIRECTORY)

# Where the UIC-created headers reside
UIC_DIR=$(BALL_PATH)/include/BALL/VIEW/UIC

THISLIB=$(LIBNAME)

CPP_SOURCES=\
	assignBondOrderConfigurationDialog.C\
	assignBondOrderResultsDialog.C\
	atomOverview.C\
	amberConfigurationDialog.C\
	bondProperties.C\
	charmmConfigurationDialog.C\
	contourSurfaceDialog.C\
	coloringSettingsDialog.C\
	compositeProperties.C\
	clippingDialog.C\
	dockDialog.C\
	dockProgressDialog.C\
	dockResultDialog.C\
	downloadPDBFile.C\
	editSettings.C\
	editSingleShortcut.C\
	displayProperties.C\
	fieldLinesDialog.C\
	FDPBDialog.C\
	gridVisualizationDialog.C\
	labelDialog.C\
	lightSettings.C\
	mainControlPreferences.C\
	materialSettings.C\
	minimizationDialog.C\
	modelSettingsDialog.C\
	modifyRepresentationDialog.C\
	molecularDynamicsDialog.C\
	molecularFileDialog.C\
	MMFF94ConfigurationDialog.C\
	networkPreferences.C\
	openSavePreferences.C\
	parsedFunctionDialog.C\
	peptideDialog.C\
	pluginDialog.C\
	preferences.C\
	exportGeometryDialog.C\
	PTEDialog.C\
	pubchemDialog.C\
	raytraceableContourSurfaceDialog.C\
	setCamera.C\
	setClippingPlane.C\
	serverPreferences.C\
	shortcutDialog.C\
	snapShotVisualisation.C\
	stageSettings.C\
	stereoSettingsDialog.C
#	openBruker1DFile.C\
#	openBruker2DFile.C

PYTHON_CPP_SOURCES=\
	pythonSettings.C

MOC_SOURCES=\
	assignBondOrderConfigurationDialog_moc.C\
	assignBondOrderResultsDialog_moc.C\
	amberConfigurationDialog_moc.C\
	atomOverview_moc.C\
	minimizationDialog_moc.C\
	compositeProperties_moc.C\
	bondProperties_moc.C\
	charmmConfigurationDialog_moc.C\
	clippingDialog_moc.C\
	coloringSettingsDialog_moc.C\
	contourSurfaceDialog_moc.C\
	dockDialog_moc.C\
	dockProgressDialog_moc.C\
	dockResultDialog_moc.C\
	downloadPDBFile_moc.C\
 	editSettings_moc.C\
	editSingleShortcut_moc.C\
	displayProperties_moc.C\
	FDPBDialog_moc.C\
	gridVisualizationDialog_moc.C\
	fieldLinesDialog_moc.C\
	labelDialog_moc.C\
	lightSettings_moc.C\
	mainControlPreferences_moc.C\
	materialSettings_moc.C\
	modelSettingsDialog_moc.C\
	modifyRepresentationDialog_moc.C\
	molecularFileDialog_moc.C\
	molecularDynamicsDialog_moc.C\
	MMFF94ConfigurationDialog_moc.C\
	networkPreferences_moc.C\
	openSavePreferences_moc.C\
	parsedFunctionDialog_moc.C\
	peptideDialog_moc.C\
	pluginDialog_moc.C\
	preferences_moc.C\
	exportGeometryDialog_moc.C\
	PTEDialog_moc.C\
	pubchemDialog_moc.C\
	raytraceableContourSurfaceDialog_moc.C\
	serverPreferences_moc.C\
	setCamera_moc.C\
	setClippingPlane_moc.C\
	shortcutDialog_moc.C\
	snapShotVisualisation_moc.C\
	stageSettings_moc.C\
	stereoSettingsDialog_moc.C
#	openBruker1DFile_moc.C\
#	openBruker2DFile_moc.C\

PYTHON_MOC_SOURCES=\
	pythonSettings_moc.C

UIC_SOURCES=\
	assignBondOrderConfigurationDialogData.C\
	assignBondOrderResultsDialogData.C\
	amberConfigurationDialogData.C\
	atomOverviewData.C\
	bondPropertiesData.C\
	charmmConfigurationDialogData.C\
	clippingDialogData.C\
	coloringSettingsDialogData.C\
	contourSurfaceDialogData.C\
	compositePropertiesData.C\
	contourSurfaceDialogData.C\
	downloadPDBFileData.C\
	displayPropertiesData.C\
	editSettingsData.C\
	editSingleShortcutData.C\
	FDPBDialogData.C\
	fieldLinesDialogData.C\
	gridVisualizationDialogData.C\
	labelDialogData.C\
	lightSettingsData.C\
	mainControlPreferencesData.C\
	materialSettingsData.C\
	minimizationDialogData.C\
	modelSettingsDialogData.C\
	modifyRepresentationDialogData.C\
	molecularDynamicsDialogData.C\
	MMFF94ConfigurationDialogData.C\
	networkPreferencesData.C\
	openSavePreferencesData.C\
	parsedFunctionDialogData.C\
	peptideDialogData.C\
	pluginDialogData.C\
	preferencesData.C\
	exportGeometryDialogData.C\
	PTEDialogData.C\
	pubchemDialogData.C\
	pythonSettingsData.C\
	raytraceableContourSurfaceDialogData.C\
	setCameraData.C\
	setClippingPlaneData.C\
	shortcutDialogData.C\
	snapShotVisualisationData.C\
	stageSettingsData.C\
	stereoSettingsDialogData.C\
	dockDialogData.C\
	dockProgressDialogData.C\
	dockResultDialogData.C

ifeq ($(BALL_HAS_FFTW),true)
	CPP_SOURCES+=geometricFitDialog.C
	MOC_SOURCES+=geometricFitDialog_moc.C
	UIC_SOURCES+=geometricFitDialogData.C
endif


UIC_HDR=$(UIC_SOURCES:Data.C=.dh)

include ../../rules.mak
include ../../targets.mak
-include .Dependencies

molecularFileDialog_moc.C: $(HEADER_DIR)/molecularFileDialog.h
	$(MOC) $(HEADER_DIR)/molecularFileDialog.h -o $@

serverPreferences_moc.C: $(HEADER_DIR)/serverPreferences.h
	$(MOC) $(HEADER_DIR)/serverPreferences.h -o $@

openBruker1DFile_moc.C: $(HEADER_DIR)/openBruker1DFile.h
	$(MOC) $(HEADER_DIR)/openBruker1DFile.h -o $@

openBruker2DFile_moc.C: $(HEADER_DIR)/openBruker2DFile.h
	$(MOC) $(HEADER_DIR)/openBruker2DFile.h -o $@

depend: $(UIC_HDR) $(MOC_SOURCES)

default_clean: clean_uic

clean_uic:
	@echo "Cleaning UIC files in $(UIC_DIR)..."
	@$(RM) -f $(UIC_DIR)/*.h
