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
	mouseSensitivity.C\
	mouseSensitivityData.C\
	setCamera.C\
	setCameraData.C\
	lightSettings.C\
	lightSettingsData.C\
	stageSettings.C\
	stageSettingsData.C\
	pythonSettings.C\
	pythonSettingsData.C

MOC_SOURCES=\
	fileDialog_moc.C\
	preferences_moc.C\
	colorMeshDialog_moc.C\
	colorMeshDialogData_moc.C\
	dlgMoveOverlay_moc.C\
	dlgMoveOverlayData_moc.C\
	mouseSensitivity_moc.C\
	mouseSensitivityData_moc.C\
	setCamera_moc.C\
	setCameraData_moc.C\
	lightSettings_moc.C\
	lightSettingsData_moc.C\
	stageSettings_moc.C\
	stageSettingsData_moc.C\
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

mouseSensitivity_moc.C:	$(HEADER_DIR)/mouseSensitivity.h
	$(MOC) $< -o $@

mouseSensitivityData_moc.C:	$(HEADER_DIR)/mouseSensitivityData.h
	$(MOC) $< -o $@

$(HEADER_DIR)/mouseSensitivityData.h:	mouseSensitivityData.ui
	$(UIC) -o $@ mouseSensitivityData.ui

mouseSensitivityData.C:	$(HEADER_DIR)/mouseSensitivityData.h
	$(UIC) -o $@ -impl $(HEADER_DIR)/mouseSensitivityData.h mouseSensitivityData.ui

setCamera_moc.C:	$(HEADER_DIR)/setCamera.h
	$(MOC) $< -o $@

setCameraData_moc.C:	$(HEADER_DIR)/setCameraData.h
	$(MOC) $< -o $@

$(HEADER_DIR)/setCameraData.h:	setCameraData.ui
	$(UIC) -o $@ setCameraData.ui

setCameraData.C:	$(HEADER_DIR)/setCameraData.h
	$(UIC) -o $@ -impl $(HEADER_DIR)/setCameraData.h setCameraData.ui


lightSettings_moc.C:	$(HEADER_DIR)/lightSettings.h
	$(MOC) $< -o $@

lightSettingsData_moc.C:	$(HEADER_DIR)/lightSettingsData.h
	$(MOC) $< -o $@

$(HEADER_DIR)/lightSettingsData.h:	lightSettingsData.ui
	$(UIC) -o $@ lightSettingsData.ui

lightSettingsData.C:	$(HEADER_DIR)/lightSettingsData.h
	$(UIC) -o $@ -impl $(HEADER_DIR)/lightSettingsData.h lightSettingsData.ui


stageSettings_moc.C:	$(HEADER_DIR)/stageSettings.h
	$(MOC) $< -o $@

stageSettingsData_moc.C:	$(HEADER_DIR)/stageSettingsData.h
	$(MOC) $< -o $@

$(HEADER_DIR)/stageSettingsData.h:	stageSettingsData.ui
	$(UIC) -o $@ stageSettingsData.ui

stageSettingsData.C:	$(HEADER_DIR)/stageSettingsData.h
	$(UIC) -o $@ -impl $(HEADER_DIR)/stageSettingsData.h stageSettingsData.ui


pythonSettings_moc.C:	$(HEADER_DIR)/pythonSettings.h
	$(MOC) $< -o $@

pythonSettingsData_moc.C:	$(HEADER_DIR)/pythonSettingsData.h
	$(MOC) $< -o $@

$(HEADER_DIR)/pythonSettingsData.h:	pythonSettingsData.ui
	$(UIC) -o $@ pythonSettingsData.ui

pythonSettingsData.C:	$(HEADER_DIR)/pythonSettingsData.h
	$(UIC) -o $@ -impl $(HEADER_DIR)/pythonSettingsData.h pythonSettingsData.ui
