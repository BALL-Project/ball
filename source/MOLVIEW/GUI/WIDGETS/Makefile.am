include ../../../Make.rules.in

DIRECTORY=MOLVIEW/GUI/WIDGETS
_objects_LINK = ../../../config/genobjects.sh $(DIRECTORY)

MOC_SOURCES=\
	molecularControl_moc.C

BUILT_SOURCES=$(MOC_SOURCES)

CURRENTDIR_SOURCES=\
	molecularControl.C\
	molecularProperties.C\
	$(MOC_SOURCES)

clean-local:
	$(RM) -f $(MOC_SOURCES)

BALL_INCLUDES=$(BALLVIEW_INCLUDES)

molecularControl_moc.C:	$(BALL_PATH)/include/BALL/MOLVIEW/GUI/WIDGETS/molecularControl.h
	$(MOC) $(BALL_PATH)/include/BALL/MOLVIEW//GUI/WIDGETS/molecularControl.h -o molecularControl_moc.C

