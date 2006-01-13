#
# $Id: Makefile,v 1.53.2.1 2006/01/13 15:35:41 amoll Exp $
#

DIRECTORY=VIEW/DIALOGS

# Where the headers of the derived classes reside
HEADER_DIR=$(BALL_PATH)/include/BALL/$(DIRECTORY)

# Where the UIC-created headers reside
UIC_DIR=$(BALL_PATH)/include/BALL/VIEW/UIC

THISLIB=$(LIBNAME)

FFT_DEPENDENT_SOURCES=$(BALL_HAS_FFTW:true=geometricFitDialog.C geometricFitDialogData.C)
FFT_DEPENDENT_MOC_SOURCES=$(BALL_HAS_FFTW:true=geometricFitDialog_moc.C geometricFitDialogData_moc.C)
FFT_DEPENDENT_UIC_SOURCES=$(BALL_HAS_FFTW:true=geometricFitDialogData.C)


CPP_SOURCES=\
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
	stageSettings.C\
	$(FFT_DEPENDENT_SOURCES)

#	openBruker1DFile.C\
#	openBruker2DFile.C\

MOC_SOURCES=\
	amberConfigurationDialog_moc.C\
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
	stageSettings_moc.C\
	$(FFT_DEPENDENT_MOC_SOURCES)
#	openBruker1DFile_moc.C\
#	openBruker2DFile_moc.C\

UIC_SOURCES=$(FFT_DEPENDENT_UIC_SOURCES)\
	amberConfigurationDialog.ui\
	bondProperties.ui\
	charmmConfigurationDialog.ui\
	clippingDialog.ui\
	coloringSettingsDialog.ui\
	contourSurfaceDialog.ui\
	compositeProperties.ui\
	contourSurfaceDialog.ui\
	downloadPDBFile.ui\
	displayProperties.ui\
	FDPBDialog.ui\
	labelDialog.ui\
	lightSettings.ui\
	mainControlPreferences.ui\
	materialSettings.ui\
	minimizationDialog.ui\
	modelSettingsDialog.ui\
	modifySurfaceDialog.ui\
	molecularDynamicsDialog.ui\
	networkPreferences.ui\
	parsedFunctionDialog.ui\
	peptideDialog.ui\
	preferences.ui\
	pythonSettings.ui\
	setCamera.ui\
	snapShotVisualisation.ui\
	stageSettings.ui\
	dockDialog.ui\
	dockProgressDialog.ui\
	dockResultDialog.ui

UIC_HDR=$(UIC_SOURCES:Data.C=.dh)

include ../../common.mak
-include .Dependencies

BALL_INCLUDES=$(VIEW_INCLUDES)

molecularFileDialog_moc.C: $(HEADER_DIR)/molecularFileDialog.h
	$(MOC) $(HEADER_DIR)/molecularFileDialog.h -o $@

serverPreferences_moc.C: $(HEADER_DIR)/serverPreferences.h
	$(MOC) $(HEADER_DIR)/serverPreferences.h -o $@

openBruker1DFile_moc.C: $(HEADER_DIR)/openBruker1DFile.h
	$(MOC) $(HEADER_DIR)/openBruker1DFile.h -o $@

openBruker2DFile_moc.C: $(HEADER_DIR)/openBruker2DFile.h
	$(MOC) $(HEADER_DIR)/openBruker2DFile.h -o $@

colorChooser_moc.C: $(HEADER_DIR)/colorChooser.h
	$(MOC) $(HEADER_DIR)/colorChooser.h -o $@

depend: $(UIC_HDR) $(MOC_SOURCES)

default_clean: clean_uic

clean_uic:
	@echo "Cleaning UIC files in $(UIC_DIR)..."
	@$(RM) -f $(UIC_DIR)/*.h
