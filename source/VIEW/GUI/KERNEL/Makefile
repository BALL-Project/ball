DIRECTORY=VIEW/GUI/KERNEL

THISLIB=$(LIBNAME)

CPP_SOURCES=\
	mainControl.C\
	objectCreator.C\
	server.C\
	representation.C\
	primitiveManager.C\
	stage.C\
	renderer.C

MOC_SOURCES=\
	mainControl_moc.C

include ../../../common.mak
include .Dependencies

BALL_INCLUDES=$(BALLVIEW_INCLUDES)

mainControl_moc.C:  $(BALL_PATH)/include/BALL/VIEW/GUI/KERNEL/mainControl.h
	$(MOC) $(BALL_PATH)/include/BALL/VIEW/GUI/KERNEL/mainControl.h -o mainControl_moc.C
 
