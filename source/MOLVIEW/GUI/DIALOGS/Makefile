DIRECTORY=MOLVIEW/GUI/DIALOGS

THISLIB=$(LIBNAME)

CPP_SOURCES=\
	openHINFile.C\
	openPDBFile.C

MOC_SOURCES=\
	openHINFile_moc.C\
	openPDBFile_moc.C

include ../../../common.mak
include .Dependencies

BALL_INCLUDES=$(BALLVIEW_INCLUDES)

openHINFile_moc.C:	$(BALL_PATH)/include/BALL/MOLVIEW/GUI/DIALOGS/openHINFile.h
	$(MOC) $(BALL_PATH)/include/BALL/MOLVIEW/GUI/DIALOGS/openHINFile.h -o openHINFile_moc.C

openPDBFile_moc.C:	$(BALL_PATH)/include/BALL/MOLVIEW/GUI/DIALOGS/openPDBFile.h
	$(MOC) $(BALL_PATH)/include/BALL/MOLVIEW/GUI/DIALOGS/openPDBFile.h -o openPDBFile_moc.C

