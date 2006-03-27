# -*- Mode: C++; tab-width: 2; -*-
# vi: set ts=2:
#
# $Id: Makefile,v 1.53.2.7 2006/03/27 13:09:26 amoll Exp $
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
	displayProperties.C\
	FDPBDialog.C\
	labelDialog.C\
	lightSettings.C\
	mainControlPreferences.C\
	materialSettings.C\
	minimizationDialog.C\
	modelSettingsDialog.C\
	modifySurfaceDialog.C\
	molecularDynamicsDialog.C\
	molecularFileDialog.C\
	networkPreferences.C\
	parsedFunctionDialog.C\
	peptideDialog.C\
	preferences.C\
	pythonSettings.C\
	setCamera.C\
	serverPreferences.C\
	snapShotVisualisation.C\
	stageSettings.C
#	openBruker1DFile.C\
#	openBruker2DFile.C

MOC_SOURCES=\
	amberConfigurationDialog_moc.C\
	atomOverview_moc.C\
	minimizationDialog_moc.C\
	compositeProperties_moc.C\
	bondProperties_moc.C\
	charmmConfigurationDialog_moc.C\
	clippingDialog_moc.C\
	coloringSettingsDialog_moc.C\
	modifySurfaceDialog_moc.C\
	contourSurfaceDialog_moc.C\
	dockDialog_moc.C\
	dockProgressDialog_moc.C\
	dockResultDialog_moc.C\
	downloadPDBFile_moc.C\
	displayProperties_moc.C\
	FDPBDialog_moc.C\
	labelDialog_moc.C\
	lightSettings_moc.C\
	mainControlPreferences_moc.C\
	materialSettings_moc.C\
	modelSettingsDialog_moc.C\
	molecularFileDialog_moc.C\
	molecularDynamicsDialog_moc.C\
	networkPreferences_moc.C\
	parsedFunctionDialog_moc.C\
	peptideDialog_moc.C\
	preferences_moc.C\
	pythonSettings_moc.C\
	serverPreferences_moc.C\
	setCamera_moc.C\
	snapShotVisualisation_moc.C\
	stageSettings_moc.C
#	openBruker1DFile_moc.C\
#	openBruker2DFile_moc.C\

UIC_SOURCES=\
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
	FDPBDialogData.C\
	labelDialogData.C\
	lightSettingsData.C\
	mainControlPreferencesData.C\
	materialSettingsData.C\
	minimizationDialogData.C\
	modelSettingsDialogData.C\
	modifySurfaceDialogData.C\
	molecularDynamicsDialogData.C\
	networkPreferencesData.C\
	parsedFunctionDialogData.C\
	peptideDialogData.C\
	preferencesData.C\
	pythonSettingsData.C\
	setCameraData.C\
	snapShotVisualisationData.C\
	stageSettingsData.C\
	dockDialogData.C\
	dockProgressDialogData.C\
	dockResultDialogData.C

ifeq ($(BALL_HAS_FFTW),true)
	CPP_SOURCES+=geometricFitDialog.C geometricFitDialogData.C
	MOC_SOURCES+=geometricFitDialog_moc.C geometricFitDialogData_moc.C
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
