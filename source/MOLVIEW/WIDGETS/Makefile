DIRECTORY=MOLVIEW/WIDGETS

THISLIB=$(LIBNAME)

CPP_SOURCES=\
	molecularControl.C\
	molecularProperties.C

MOC_SOURCES=\
	molecularControl_moc.C\
	molecularProperties_moc.C


include ../../common.mak
include .Dependencies

BALL_INCLUDES=$(BALLVIEW_INCLUDES)

molecularControl_moc.C:	$(BALL_PATH)/include/BALL/MOLVIEW/WIDGETS/molecularControl.h
	$(MOC) $(BALL_PATH)/include/BALL/MOLVIEW/WIDGETS/molecularControl.h -o molecularControl_moc.C

molecularProperties_moc.C:	$(BALL_PATH)/include/BALL/MOLVIEW/WIDGETS/molecularProperties.h
	$(MOC) $(BALL_PATH)/include/BALL/MOLVIEW/WIDGETS/molecularProperties.h -o molecularProperties_moc.C
