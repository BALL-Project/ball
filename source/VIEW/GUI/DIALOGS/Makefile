DIRECTORY=VIEW/GUI/DIALOGS

THISLIB=$(LIBNAME)

CPP_SOURCES=\
	fileDialog.C

MOC_SOURCES=\
	fileDialog_moc.C

include ../../../common.mak
include .Dependencies

BALL_INCLUDES=$(BALLVIEW_INCLUDES)

fileDialog_moc.C:	$(BALL_PATH)/include/BALL/VIEW/GUI/DIALOGS/fileDialog.h
	$(MOC) $(BALL_PATH)/include/BALL/VIEW/GUI/DIALOGS/fileDialog.h -o fileDialog_moc.C

