DIRECTORY=VIEW/GUI/DIALOGS

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
	colorMeshDialogData_moc.C

include ../../../common.mak
include .Dependencies

BALL_INCLUDES=$(BALLVIEW_INCLUDES)

fileDialog_moc.C:	$(BALL_PATH)/include/BALL/VIEW/GUI/DIALOGS/fileDialog.h
	$(MOC) $(BALL_PATH)/include/BALL/VIEW/GUI/DIALOGS/fileDialog.h -o fileDialog_moc.C

preferences_moc.C:	$(BALL_PATH)/include/BALL/VIEW/GUI/DIALOGS/preferences.h
	$(MOC) $(BALL_PATH)/include/BALL/VIEW/GUI/DIALOGS/preferences.h -o preferences_moc.C

colorMeshDialogData_moc.C:	$(BALL_PATH)/include/BALL/VIEW/GUI/DIALOGS/colorMeshDialogData.h
	$(MOC) $(BALL_PATH)/include/BALL/VIEW/GUI/DIALOGS/colorMeshDialogData.h -o colorMeshDialogData_moc.C

colorMeshDialog_moc.C:	$(BALL_PATH)/include/BALL/VIEW/GUI/DIALOGS/colorMeshDialog.h
	$(MOC) $(BALL_PATH)/include/BALL/VIEW/GUI/DIALOGS/colorMeshDialog.h -o colorMeshDialog_moc.C


