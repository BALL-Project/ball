DIRECTORY=MOLVIEW/GUI/WIDGETS

THISLIB=$(LIBNAME)

CPP_SOURCES=\
	molecularControl.C\
	molecularProperties.C

MOC_SOURCES=\
	molecularControl_moc.C


include ../../../common.mak
include .Dependencies

BALL_INCLUDES=$(BALLVIEW_INCLUDES)

molecularControl_moc.C:	$(BALL_PATH)/include/BALL/MOLVIEW/GUI/WIDGETS/molecularControl.h
	$(MOC) $(BALL_PATH)/include/BALL/MOLVIEW//GUI/WIDGETS/molecularControl.h -o molecularControl_moc.C

