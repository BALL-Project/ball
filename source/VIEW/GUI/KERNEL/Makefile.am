include ../../../Make.rules.in

DIRECTORY=VIEW/GUI/KERNEL
_objects_LINK = ../../../config/genobjects.sh $(DIRECTORY)

MOC_SOURCES=\
	mainControl_moc.C

BUILT_SOURCES=$(MOC_SOURCES)

CURRENTDIR_SOURCES=\
	compositeDescriptor.C\
	glDisplayList.C\
	glEntityDescriptor.C\
	glObject.C\
	glPrimitiveManager.C\
	glQuadricObject.C\
	mainControl.C\
	objectCreator.C\
	server.C\
	$(MOC_SOURCES)

clean-local:
	$(RM) -f $(MOC_SOURCES)
	
BALL_INCLUDES=$(BALLVIEW_INCLUDES)

mainControl_moc.C:  $(BALL_PATH)/include/BALL/VIEW/GUI/KERNEL/mainControl.h
	$(MOC) $(BALL_PATH)/include/BALL/VIEW/GUI/KERNEL/mainControl.h -o mainControl_moc.C
