DIRECTORY=VIEW/GUI/DIALOGS

HEADER_DIR=$(BALL_PATH)/include/BALL/$(DIRECTORY)

THISLIB=$(LIBNAME)

CPP_SOURCES=\
	fileDialog.C\
	preferences.C\
	colorMeshDialog.C\
	colorMeshDialogData.C

MOC_SOURCES=\
	fileDialog_moc.C\
	preferences_moc.C\
	colorMeshDialog_moc.C\
	colorMeshDialogData_moc.C\

include ../../../common.mak
include .Dependencies

BALL_INCLUDES=$(BALLVIEW_INCLUDES)

fileDialog_moc.C:	$(HEADER_DIR)/fileDialog.h
	$(MOC) $< -o $@

preferences_moc.C:	$(HEADER_DIR)/preferences.h
	$(MOC) $< -o $@

colorMeshDialog_moc.C:	$(HEADER_DIR)/colorMeshDialog.h
	$(MOC) $< -o $@

colorMeshDialogData_moc.C:	$(HEADER_DIR)/colorMeshDialogData.h
	$(MOC) $< -o $@

$(HEADER_DIR)/colorMeshDialogData.h:	colorMeshDialog.ui
	$(UIC) -o $@ $<

colorMeshDialogData.C:	$(HEADER_DIR)/colorMeshDialogData.h
	$(UIC) -o $@ -impl $< colorMeshDialog.ui
