DIRECTORY=MOLVIEW/GUI/DIALOGS

THISLIB=$(LIBNAME)

CPP_SOURCES=\
	displayProperties.C\
	displayPropertiesData.C\
	labelProperties.C\
	labelPropertiesData.C\
	openHINFile.C\
	openPDBFile.C\
	openBruker2DFile.C

MOC_SOURCES=\
	displayProperties_moc.C\
	displayPropertiesData_moc.C\
	labelProperties_moc.C\
	labelPropertiesData_moc.C\
	openHINFile_moc.C\
	openPDBFile_moc.C\
	openBruker2DFile_moc.C

include ../../../common.mak
include .Dependencies

BALL_INCLUDES=$(BALLVIEW_INCLUDES)

displayProperties_moc.C:	$(BALL_PATH)/include/BALL/MOLVIEW/GUI/DIALOGS/displayProperties.h
	$(MOC) $(BALL_PATH)/include/BALL/MOLVIEW/GUI/DIALOGS/displayProperties.h -o displayProperties_moc.C

displayPropertiesData_moc.C:	$(BALL_PATH)/include/BALL/MOLVIEW/GUI/DIALOGS/displayPropertiesData.h
	$(MOC) $(BALL_PATH)/include/BALL/MOLVIEW/GUI/DIALOGS/displayPropertiesData.h -o displayPropertiesData_moc.C

labelProperties_moc.C:	$(BALL_PATH)/include/BALL/MOLVIEW/GUI/DIALOGS/labelProperties.h
	$(MOC) $(BALL_PATH)/include/BALL/MOLVIEW/GUI/DIALOGS/labelProperties.h -o labelProperties_moc.C

labelPropertiesData_moc.C:	$(BALL_PATH)/include/BALL/MOLVIEW/GUI/DIALOGS/labelPropertiesData.h
	$(MOC) $(BALL_PATH)/include/BALL/MOLVIEW/GUI/DIALOGS/labelPropertiesData.h -o labelPropertiesData_moc.C

openHINFile_moc.C:	$(BALL_PATH)/include/BALL/MOLVIEW/GUI/DIALOGS/openHINFile.h
	$(MOC) $(BALL_PATH)/include/BALL/MOLVIEW/GUI/DIALOGS/openHINFile.h -o openHINFile_moc.C

openPDBFile_moc.C:	$(BALL_PATH)/include/BALL/MOLVIEW/GUI/DIALOGS/openPDBFile.h
	$(MOC) $(BALL_PATH)/include/BALL/MOLVIEW/GUI/DIALOGS/openPDBFile.h -o openPDBFile_moc.C

openBruker2DFile_moc.C:	$(BALL_PATH)/include/BALL/MOLVIEW/GUI/DIALOGS/openBruker2DFile.h
	$(MOC) $(BALL_PATH)/include/BALL/MOLVIEW/GUI/DIALOGS/openBruker2DFile.h -o openBruker2DFile_moc.C
