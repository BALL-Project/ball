include ../../../Make.rules.in

DIRECTORY=MOLVIEW/GUI/DIALOGS
_objects_LINK = ../../../config/genobjects.sh $(DIRECTORY)

MOC_SOURCES=\
	displayProperties_moc.C\
	displayPropertiesData_moc.C\
	labelProperties_moc.C\
	labelPropertiesData_moc.C\
	contourSurfaceDialog_moc.C\
	atomProperties_moc.C\
	atomPropertiesData_moc.C\
	molecularFileDialog_moc.C

BUILT_SOURCES=$(MOC_SOURCES)

CURRENTDIR_SOURCES=\
	displayProperties.C\
	displayPropertiesData.C\
	labelProperties.C\
	labelPropertiesData.C\
	openHINFile.C\
	openMOLFile.C\
	openMOL2File.C\
	openPDBFile.C\
	openBruker1DFile.C\
	openBruker2DFile.C\
	contourSurfaceDialog.C\
	atomProperties.C\
	atomPropertiesData.C\
	molecularFileDialog.C\
	$(MOC_SOURCES)

clean-local:
	$(RM) -f $(MOC_SOURCES)

BALL_INCLUDES=$(BALLVIEW_INCLUDES)

displayProperties_moc.C:	$(BALL_PATH)/include/BALL/MOLVIEW/GUI/DIALOGS/displayProperties.h
	$(MOC) $(BALL_PATH)/include/BALL/MOLVIEW/GUI/DIALOGS/displayProperties.h -o displayProperties_moc.C

displayPropertiesData_moc.C:	$(BALL_PATH)/include/BALL/MOLVIEW/GUI/DIALOGS/displayPropertiesData.h
	$(MOC) $(BALL_PATH)/include/BALL/MOLVIEW/GUI/DIALOGS/displayPropertiesData.h -o displayPropertiesData_moc.C

labelProperties_moc.C:	$(BALL_PATH)/include/BALL/MOLVIEW/GUI/DIALOGS/labelProperties.h
	$(MOC) $(BALL_PATH)/include/BALL/MOLVIEW/GUI/DIALOGS/labelProperties.h -o labelProperties_moc.C

labelPropertiesData_moc.C:	$(BALL_PATH)/include/BALL/MOLVIEW/GUI/DIALOGS/labelPropertiesData.h
	$(MOC) $(BALL_PATH)/include/BALL/MOLVIEW/GUI/DIALOGS/labelPropertiesData.h -o labelPropertiesData_moc.C

contourSurfaceDialog_moc.C:	$(BALL_PATH)/include/BALL/MOLVIEW/GUI/DIALOGS/contourSurfaceDialog.h
	$(MOC) $(BALL_PATH)/include/BALL/MOLVIEW/GUI/DIALOGS/contourSurfaceDialog.h -o contourSurfaceDialog_moc.C

atomProperties_moc.C: $(BALL_PATH)/include/BALL/MOLVIEW/GUI/DIALOGS/atomProperties.h
	$(MOC) $(BALL_PATH)/include/BALL/MOLVIEW/GUI/DIALOGS/atomProperties.h -o atomProperties_moc.C

atomPropertiesData_moc.C: $(BALL_PATH)/include/BALL/MOLVIEW/GUI/DIALOGS/atomPropertiesData.h
	$(MOC) $(BALL_PATH)/include/BALL/MOLVIEW/GUI/DIALOGS/atomPropertiesData.h -o atomPropertiesData_moc.C
	
molecularFileDialog_moc.C: $(BALL_PATH)/include/BALL/MOLVIEW/GUI/DIALOGS/molecularFileDialog.h
	$(MOC) $(BALL_PATH)/include/BALL/MOLVIEW/GUI/DIALOGS/molecularFileDialog.h -o molecularFileDialog_moc.C
