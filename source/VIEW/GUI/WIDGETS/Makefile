DIRECTORY=VIEW/GUI/WIDGETS

THISLIB=$(LIBNAME)

CPP_SOURCES=\
	control.C\
	modularWidget.C\
	scene.C

PYTHON_CPP_SOURCES=\
	pyWidget.C

MOC_SOURCES=\
	control_moc.C\
	scene_moc.C

PYTHON_MOC_SOURCES=\
	pyWidget_moc.C

include ../../../common.mak
include .Dependencies

BALL_INCLUDES=$(BALLVIEW_INCLUDES) $(PYTHON_INCLUDES)

control_moc.C:	$(BALL_PATH)/include/BALL/VIEW/GUI/WIDGETS/control.h
	$(MOC) $(BALL_PATH)/include/BALL/VIEW/GUI/WIDGETS/control.h -o control_moc.C

scene_moc.C:	$(BALL_PATH)/include/BALL/VIEW/GUI/WIDGETS/scene.h
	$(MOC) $(BALL_PATH)/include/BALL/VIEW/GUI/WIDGETS/scene.h -o scene_moc.C

pyWidget_moc.C:	$(BALL_PATH)/include/BALL/VIEW/GUI/WIDGETS/pyWidget.h
	$(MOC) $(BALL_PATH)/include/BALL/VIEW/GUI/WIDGETS/pyWidget.h -o pyWidget_moc.C



