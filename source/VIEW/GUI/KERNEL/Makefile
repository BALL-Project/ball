DIRECTORY=VIEW/GUI/KERNEL

THISLIB=$(LIBNAME)

CPP_SOURCES=\
	compositeDescriptor.C\
	glDisplayList.C\
	glEntityDescriptor.C\
	glObject.C\
	glPrimitiveManager.C\
	glQuadricObject.C\
	mainControl.C\
	objectCreator.C\
	server.C

MOC_SOURCES=\
	mainControl_moc.C

MOC=/KM/comp-bio/BALL-contrib/qt/bin/IRIX6-CC_7.2.1/moc

include ../../../common.mak
include .Dependencies

BALL_INCLUDES=$(BALLVIEW_INCLUDES)

mainControl_moc.C:  $(BALL_PATH)/include/BALL/VIEW/GUI/KERNEL/mainControl.h
	$(MOC) $(BALL_PATH)/include/BALL/VIEW/GUI/KERNEL/mainControl.h -o mainControl_moc.C
 
