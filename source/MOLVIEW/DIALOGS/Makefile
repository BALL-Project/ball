DIRECTORY=MOLVIEW/DIALOGS

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
	contourSurfaceDialog.C\
	contourSurfaceDialogData.C\
	amberMinimizationDialog.C\
	amberMinimizationDialogData.C\
	molecularDynamicsDialog.C\
	molecularDynamicsDialogData.C\
	peptideDialog.C\
	peptideDialogData.C\
	snapShotVisualisation.C\
	snapShotVisualisationData.C

MOC_SOURCES=\
	atomProperties_moc.C\
	atomPropertiesData_moc.C\
	displayProperties_moc.C\
	displayPropertiesData_moc.C\
	labelProperties_moc.C\
	labelPropertiesData_moc.C\
	molecularFileDialog_moc.C\
	contourSurfaceDialog_moc.C\
	contourSurfaceDialogData_moc.C\
	amberMinimizationDialog_moc.C\
	amberMinimizationDialogData_moc.C\
	molecularDynamicsDialog_moc.C\
	molecularDynamicsDialogData_moc.C\
	peptideDialog_moc.C\
	peptideDialogData_moc.C\
	snapShotVisualisation_moc.C\
	snapShotVisualisationData_moc.C


include ../../common.mak
include .Dependencies

BALL_INCLUDES=$(BALLVIEW_INCLUDES)

displayProperties_moc.C:	$(HEADER_DIR)/displayProperties.h
	$(MOC) $(HEADER_DIR)/displayProperties.h -o $@

displayPropertiesData_moc.C:	$(HEADER_DIR)/displayPropertiesData.h
	$(MOC) $(HEADER_DIR)/displayPropertiesData.h -o $@ 

$(HEADER_DIR)/displayPropertiesData.h:	displayProperties.ui
	$(UIC) -o $@ displayProperties.ui

displayPropertiesData.C:	$(HEADER_DIR)/displayPropertiesData.h
	$(UIC) -o $@ -impl $(HEADER_DIR)/displayPropertiesData.h	displayProperties.ui

labelProperties_moc.C:	$(HEADER_DIR)/labelProperties.h
	$(MOC) $(HEADER_DIR)/labelProperties.h -o $@

labelPropertiesData_moc.C:	$(HEADER_DIR)/labelPropertiesData.h
	$(MOC) $(HEADER_DIR)/labelPropertiesData.h -o $@

openBruker1DFile_moc.C: $(HEADER_DIR)/openBruker1DFile.h
	$(MOC) $(HEADER_DIR)/openBruker1DFile.h -o $@

openBruker2DFile_moc.C:	$(HEADER_DIR)/openBruker2DFile.h
	$(MOC) $(HEADER_DIR)/openBruker2DFile.h -o $@

molecularFileDialog_moc.C:	$(HEADER_DIR)/molecularFileDialog.h
	$(MOC) $(HEADER_DIR)/molecularFileDialog.h -o $@

contourSurfaceDialog_moc.C:	$(HEADER_DIR)/contourSurfaceDialog.h
	$(MOC) $(HEADER_DIR)/contourSurfaceDialog.h -o $@

contourSurfaceDialogData_moc.C:	$(HEADER_DIR)/contourSurfaceDialogData.h
	$(MOC) $(HEADER_DIR)/contourSurfaceDialogData.h -o $@

atomProperties_moc.C:	$(HEADER_DIR)/atomProperties.h
	$(MOC) $(HEADER_DIR)/atomProperties.h -o $@

atomPropertiesData_moc.C:	$(HEADER_DIR)/atomPropertiesData.h
	$(MOC) $(HEADER_DIR)/atomPropertiesData.h -o $@

$(HEADER_DIR)/atomPropertiesData.h:	atomProperties.ui
	$(UIC) -o $@ atomProperties.ui

atomPropertiesData.C:	$(HEADER_DIR)/atomPropertiesData.h
	$(UIC) -o $@ -impl $(HEADER_DIR)/atomPropertiesData.h atomProperties.ui


amberMinimizationDialog_moc.C:	$(HEADER_DIR)/amberMinimizationDialog.h
	$(MOC) $(HEADER_DIR)/amberMinimizationDialog.h -o $@

amberMinimizationDialogData_moc.C:	$(HEADER_DIR)/amberMinimizationDialogData.h
	$(MOC) $(HEADER_DIR)/amberMinimizationDialogData.h -o $@

$(HEADER_DIR)/amberMinimizationDialogData.h:  amberMinimizationDialog.ui
	$(UIC) -o $@ amberMinimizationDialog.ui

amberMinimizationDialogData.C:  $(HEADER_DIR)/amberMinimizationDialogData.h
	$(UIC) -o $@ -impl $(HEADER_DIR)/amberMinimizationDialogData.h amberMinimizationDialog.ui


molecularDynamicsDialog_moc.C:	$(HEADER_DIR)/molecularDynamicsDialog.h
	$(MOC) $(HEADER_DIR)/molecularDynamicsDialog.h -o $@

molecularDynamicsDialogData_moc.C:	$(HEADER_DIR)/molecularDynamicsDialogData.h
	$(MOC) $(HEADER_DIR)/molecularDynamicsDialogData.h -o $@

$(HEADER_DIR)/molecularDynamicsDialogData.h:  molecularDynamicsDialog.ui
	$(UIC) -o $@ molecularDynamicsDialog.ui

molecularDynamicsDialogData.C:  $(HEADER_DIR)/molecularDynamicsDialogData.h
	$(UIC) -o $@ -impl $(HEADER_DIR)/molecularDynamicsDialogData.h  molecularDynamicsDialog.ui


peptideDialog_moc.C:	$(HEADER_DIR)/peptideDialog.h
	$(MOC) $(HEADER_DIR)/peptideDialog.h -o $@

peptideDialogData_moc.C:	$(HEADER_DIR)/peptideDialogData.h
	$(MOC) $(HEADER_DIR)/peptideDialogData.h -o $@

$(HEADER_DIR)/peptideDialogData.h:	peptideDialog.ui
	$(UIC) -o $@ peptideDialog.ui

peptideDialogData.C:	$(HEADER_DIR)/peptideDialogData.h
	$(UIC) -o $@ -impl $(HEADER_DIR)/peptideDialogData.h peptideDialog.ui


snapShotVisualisation_moc.C:	$(HEADER_DIR)/snapShotVisualisation.h
	$(MOC) $(HEADER_DIR)/snapShotVisualisation.h -o $@

snapShotVisualisationData_moc.C:	$(HEADER_DIR)/snapShotVisualisationData.h
	$(MOC) $(HEADER_DIR)/snapShotVisualisationData.h -o $@

$(HEADER_DIR)/snapShotVisualisationData.h:	snapShotVisualisation.ui
	$(UIC) -o $@ snapShotVisualisation.ui

snapShotVisualisationData.C:	$(HEADER_DIR)/snapShotVisualisationData.h
	$(UIC) -o $@ -impl $(HEADER_DIR)/snapShotVisualisationData.h snapShotVisualisation.ui
