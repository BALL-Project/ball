DIRECTORY=VIEW/GUI/DIALOGS

HEADER_DIR=$(BALL_PATH)/include/BALL/$(DIRECTORY)

THISLIB=$(LIBNAME)

CPP_SOURCES=\
	fileDialog.C\
	preferences.C\
	colorMeshDialog.C\
	colorMeshDialogData.C\
	dlgMoveOverlay.C\
	dlgMoveOverlayData.C\
	pythonSettings.C\
	pythonSettingsData.C

MOC_SOURCES=\
	fileDialog_moc.C\
	preferences_moc.C\
	colorMeshDialog_moc.C\
	colorMeshDialogData_moc.C\
	dlgMoveOverlay_moc.C\
	dlgMoveOverlayData_moc.C\
	pythonSettings_moc.C\
	pythonSettingsData_moc.C

include ../../../common.mak
include .Dependencies

BALL_INCLUDES=$(BALLVIEW_INCLUDES)

fileDialog_moc.C:	$(HEADER_DIR)/fileDialog.h
	$(MOC) $< -o $@

preferences_moc.C:	$(HEADER_DIR)/preferences.h
	$(MOC) $< -o $@

dlgMoveOverlay_moc.C:	$(HEADER_DIR)/dlgMoveOverlay.h
	$(MOC) $< -o $@

dlgMoveOverlayData_moc.C:	$(HEADER_DIR)/dlgMoveOverlayData.h
	$(MOC) $< -o $@

colorMeshDialog_moc.C:	$(HEADER_DIR)/colorMeshDialog.h
	$(MOC) $< -o $@

colorMeshDialogData_moc.C:	$(HEADER_DIR)/colorMeshDialogData.h
	$(MOC) $< -o $@

colorMeshDialogData.C:	$(HEADER_DIR)/colorMeshDialogData.h
	$(UIC) -o $@ -impl $(HEADER_DIR)/colorMeshDialogData.h colorMeshDialog.ui

$(HEADER_DIR)/colorMeshDialogData.h:	colorMeshDialog.ui
	$(UIC) -o $@ colorMeshDialog.ui


pythonSettings_moc.C:	$(HEADER_DIR)/pythonSettings.h
	$(MOC) $< -o $@

pythonSettingsData_moc.C:	$(HEADER_DIR)/pythonSettingsData.h
	$(MOC) $< -o $@

$(HEADER_DIR)/pythonSettingsData.h:	pythonSettingsData.ui
	$(UIC) -o $@ pythonSettingsData.ui

pythonSettingsData.C:	$(HEADER_DIR)/pythonSettingsData.h
	$(UIC) -o $@ -impl $(HEADER_DIR)/pythonSettingsData.h pythonSettingsData.ui
