DIRECTORY=VIEW/KERNEL

THISLIB=$(LIBNAME)

# replace library-specific flags by application-
# specific compiler flags
LIB_CXXFLAGS=$(NONLIB_CXXFLAGS) -DQT_THREAD_SUPPORT

CPP_SOURCES=\
  client.C\
	common.C\
  compositeManager.C\
  connectionObject.C\
	events.C\
  geometricObject.C\
  information.C\
  mainControl.C\
  message.C\
  modularWidget.C\
  molecularInformation.C\
  moleculeObjectCreator.C\
  objectCreator.C\
  primitiveManager.C\
  QTTimer.C\
  representation.C\
  server.C\
  stage.C\
	threads.C

MOC_SOURCES=\
	mainControl_moc.C

include ../../common.mak
include .Dependencies

BALL_INCLUDES=$(VIEW_INCLUDES) -DQT_THREAD_SUPPORT

mainControl_moc.C:  $(BALL_PATH)/include/BALL/VIEW/KERNEL/mainControl.h
	$(MOC) $(BALL_PATH)/include/BALL/VIEW/KERNEL/mainControl.h -o mainControl_moc.C
 
