DIRECTORY=MOLVIEW/GUI/DIALOGS

HEADER_DIR=$(BALL_PATH)/include/BALL/$(DIRECTORY)

THISLIB=$(LIBNAME)

CPP_SOURCES=\
	atomProperties.C\
	atomPropertiesData.C\
	displayProperties.C\
	displayPropertiesData.C\
	labelProperties.C\
	labelPropertiesData.C\
	molecularFileDialog.C\
	openBruker1DFile.C\
	openBruker2DFile.C\
	contourSurfaceDialog.C\
	contourSurfaceDialogData.C\
	peptideDialog.C\
	peptideDialogData.C\
	amberMinimizationDialog.C\
	amberMinimizationDialogData.C\
	molecularDynamicsDialog.C\
	molecularDynamicsDialogData.C\

MOC_SOURCES=\
	atomProperties_moc.C\
	atomPropertiesData_moc.C\
	displayProperties_moc.C\
	displayPropertiesData_moc.C\
	labelProperties_moc.C\
	labelPropertiesData_moc.C\
	molecularFileDialog_moc.C\
	openBruker1DFile_moc.C\
	openBruker2DFile_moc.C\
	contourSurfaceDialog_moc.C\
	contourSurfaceDialogData_moc.C\
	peptideDialog_moc.C\
	peptideDialogData_moc.C\
	amberMinimizationDialog_moc.C\
	amberMinimizationDialogData_moc.C\
	molecularDynamicsDialog_moc.C\
	molecularDynamicsDialogData_moc.C

include ../../../common.mak
include .Dependencies

BALL_INCLUDES=$(BALLVIEW_INCLUDES)

labelProperties_moc.C:	$(HEADER_DIR)/labelProperties.h
	$(MOC) $< -o $@

labelPropertiesData_moc.C:	$(HEADER_DIR)/labelPropertiesData.h
	$(MOC) $< -o $@

openBruker1DFile_moc.C: $(HEADER_DIR)/openBruker1DFile.h
	$(MOC) $< -o $@

openBruker2DFile_moc.C:	$(HEADER_DIR)/openBruker2DFile.h
	$(MOC) $< -o $@

molecularFileDialog_moc.C:	$(HEADER_DIR)/molecularFileDialog.h
	$(MOC) $< -o $@

contourSurfaceDialog_moc.C:	$(HEADER_DIR)/contourSurfaceDialog.h
	$(MOC) $< -o $@

contourSurfaceDialogData_moc.C:	$(HEADER_DIR)/contourSurfaceDialogData.h
	$(MOC) $< -o $@

$(HEADER_DIR)/contourSurfaceDialogData.h:  contourSurfaceDialog.ui
	$(UIC) -o $@ $<

contourSurfaceDialogData.C:  $(HEADER_DIR)/contourSurfaceDialogData.h
	$(UIC) -o $@ -impl $< contourSurfaceDialog.ui

displayProperties_moc.C:	$(HEADER_DIR)/displayProperties.h
	$(MOC) $< -o $@

displayPropertiesData_moc.C:	$(HEADER_DIR)/displayPropertiesData.h
	$(MOC) $< -o $@

$(HEADER_DIR)/displayPropertiesData.h:  displayProperties.ui
	$(UIC) -o $@ $<

displayPropertiesData.C:  $(HEADER_DIR)/displayPropertiesData.h
	$(UIC) -o $@ -impl $< displayProperties.ui

atomProperties_moc.C:	$(HEADER_DIR)/atomProperties.h
	$(MOC) $< -o $@

atomPropertiesData_moc.C:	$(HEADER_DIR)/atomPropertiesData.h
	$(MOC) $< -o $@

$(HEADER_DIR)/atomPropertiesData.h:  atomProperties.ui
	$(UIC) -o $@ $<

atomPropertiesData.C:  $(HEADER_DIR)/atomPropertiesData.h
	$(UIC) -o $@ -impl $< atomProperties.ui


peptideDialog_moc.C:	$(HEADER_DIR)/peptideDialog.h
	$(MOC) $< -o $@

peptideDialogData_moc.C:	$(HEADER_DIR)/peptideDialogData.h
	$(MOC) $< -o $@

$(HEADER_DIR)/peptideDialogData.h:  peptideDialog.ui
	$(UIC) -o $@ $<

peptideDialogData.C:  $(HEADER_DIR)/peptideDialogData.h
	$(UIC) -o $@ -impl $< peptideDialog.ui

amberMinimizationDialog_moc.C:	$(HEADER_DIR)/amberMinimizationDialog.h
	$(MOC) $< -o $@

amberMinimizationDialogData_moc.C:	$(HEADER_DIR)/amberMinimizationDialogData.h
	$(MOC) $< -o $@

$(HEADER_DIR)/amberMinimizationDialogData.h:  amberMinimizationDialog.ui
	$(UIC) -o $@ $<

amberMinimizationDialogData.C:  $(HEADER_DIR)/amberMinimizationDialogData.h
	$(UIC) -o $@ -impl $< amberMinimizationDialog.ui


molecularDynamicsDialog_moc.C:	$(HEADER_DIR)/molecularDynamicsDialog.h
	$(MOC) $< -o $@

molecularDynamicsDialogData_moc.C:	$(HEADER_DIR)/molecularDynamicsDialogData.h
	$(MOC) $< -o $@

$(HEADER_DIR)/molecularDynamicsDialogData.h:  molecularDynamicsDialog.ui
	$(UIC) -o $@ $<

molecularDynamicsDialogData.C:  $(HEADER_DIR)/molecularDynamicsDialogData.h
	$(UIC) -o $@ -impl $< molecularDynamicsDialog.ui

