DIRECTORY=MOLVIEW/GUI/DIALOGS

HEADER_DIR=$(BALL_PATH)/include/BALL/$(DIRECTORY)

THISLIB=$(LIBNAME)

CPP_SOURCES=\
	displayProperties.C\
	displayPropertiesData.C\
	labelProperties.C\
	labelPropertiesData.C\
	molecularFileDialog.C\
	openBruker1DFile.C\
	openBruker2DFile.C\
	contourSurfaceDialog.C\
	contourSurfaceDialogData.C

MOC_SOURCES=\
	displayProperties_moc.C\
	displayPropertiesData_moc.C\
	labelProperties_moc.C\
	labelPropertiesData_moc.C\
	molecularFileDialog_moc.C\
	openBruker1DFile_moc.C\
	openBruker2DFile_moc.C\
	contourSurfaceDialog_moc.C\
	contourSurfaceDialogData_moc.C

include ../../../common.mak
include .Dependencies

BALL_INCLUDES=$(BALLVIEW_INCLUDES)

displayProperties_moc.C:	$(HEADER_DIR)/displayProperties.h
	$(MOC) $< -o $@

displayPropertiesData_moc.C:	$(HEADER_DIR)/displayPropertiesData.h
	$(MOC) $< -o $@

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

atomProperties_moc.C:	$(HEADER_DIR)/atomProperties.h
	$(MOC) $< -o $@

atomPropertiesData_moc.C:	$(HEADER_DIR)/atomPropertiesData.h
	$(MOC) $< -o $@

$(HEADER_DIR)/displayPropertiesData.h:  displayProperties.ui
	$(UIC) -o $@ $<

displayPropertiesData.C:  $(HEADER_DIR)/displayPropertiesData.h
	$(UIC) -o $@ -impl $< displayProperties.ui

$(HEADER_DIR)/atomPropertiesData.h:  atomProperties.ui
	$(UIC) -o $@ $<

atomPropertiesData.C:  $(HEADER_DIR)/atomPropertiesData.h
	$(UIC) -o $@ -impl $< atomProperties.ui

