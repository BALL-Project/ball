DIRECTORY=VIEW/GUI/WIDGETS

THISLIB=$(LIBNAME)

CPP_SOURCES=\
	control.C\
	mainControlPreferences.C\
	modularWidget.C\
	regularData1DWidget.C\
	regularData2DWidget.C\
	scene.C\
	serverPreferences.C

PYTHON_CPP_SOURCES=\
	pyWidget.C

MOC_SOURCES=\
	control_moc.C\
	scene_moc.C\
	regularData1DWidget_moc.C\
	regularData2DWidget_moc.C\
	mainControlPreferences_moc.C\
	serverPreferences_moc.C

PYTHON_MOC_SOURCES=\
	pyWidget_moc.C

include ../../../common.mak
include .Dependencies

BALL_INCLUDES=$(BALLVIEW_INCLUDES) $(PYTHON_INCLUDES)

control_moc.C:	$(BALL_PATH)/include/BALL/VIEW/GUI/WIDGETS/control.h
	$(MOC) $(BALL_PATH)/include/BALL/VIEW/GUI/WIDGETS/control.h -o control_moc.C

mainControlPreferences_moc.C:	$(BALL_PATH)/include/BALL/VIEW/GUI/WIDGETS/mainControlPreferences.h
	$(MOC) $(BALL_PATH)/include/BALL/VIEW/GUI/WIDGETS/mainControlPreferences.h -o mainControlPreferences_moc.C

scene_moc.C:	$(BALL_PATH)/include/BALL/VIEW/GUI/WIDGETS/scene.h
	$(MOC) $(BALL_PATH)/include/BALL/VIEW/GUI/WIDGETS/scene.h -o scene_moc.C

serverPreferences_moc.C:	$(BALL_PATH)/include/BALL/VIEW/GUI/WIDGETS/serverPreferences.h
	$(MOC) $(BALL_PATH)/include/BALL/VIEW/GUI/WIDGETS/serverPreferences.h -o serverPreferences_moc.C

pyWidget_moc.C:	$(BALL_PATH)/include/BALL/VIEW/GUI/WIDGETS/pyWidget.h
	$(MOC) $(BALL_PATH)/include/BALL/VIEW/GUI/WIDGETS/pyWidget.h -o pyWidget_moc.C

regularData1DWidget_moc.C:	$(BALL_PATH)/include/BALL/VIEW/GUI/WIDGETS/regularData1DWidget.h
	$(MOC) $(BALL_PATH)/include/BALL/VIEW/GUI/WIDGETS/regularData1DWidget.h -o regularData1DWidget_moc.C

regularData2DWidget_moc.C:	$(BALL_PATH)/include/BALL/VIEW/GUI/WIDGETS/regularData2DWidget.h
	$(MOC) $(BALL_PATH)/include/BALL/VIEW/GUI/WIDGETS/regularData2DWidget.h -o regularData2DWidget_moc.C
