include ../../../Make.rules.in

if PYTHON_SUPPORT
PYTHON_SRC=pyWidget.C
PYTHON_MOC_SRC=pyWidget_moc.C
else
PYTHON_SRC=
PYTHON_MOC_SRC=
endif
	
DIRECTORY=VIEW/GUI/WIDGETS
_objects_LINK = ../../../config/genobjects.sh $(DIRECTORY)

MOC_SOURCES=\
	control_moc.C\
	scene_moc.C\
	regularData1DWidget_moc.C\
	regularData2DWidget_moc.C\
	mainControlPreferences_moc.C\
	serverPreferences_moc.C\
	$(PYTHON_MOC_SRC)

CURRENTDIR_SOURCES=\
	control.C\
	mainControlPreferences.C\
	modularWidget.C\
	regularData1DWidget.C\
	regularData2DWidget.C\
	scene.C\
	serverPreferences.C\
	$(MOC_SOURCES)\
	$(PYTHON_SRC)

clean-local:
	$(RM) -f $(MOC_SOURCES)
	
BALL_INCLUDES=$(BALLVIEW_INCLUDES) $(PYTHON_INCLUDES)

control_moc.C:	$(BALL_PATH)/include/BALL/VIEW/GUI/WIDGETS/control.h
	$(MOC) $(BALL_PATH)/include/BALL/VIEW/GUI/WIDGETS/control.h -o control_moc.C

mainControlPreferences_moc.C:	$(BALL_PATH)/include/BALL/VIEW/GUI/WIDGETS/mainControlPreferences.h
	$(MOC) $(BALL_PATH)/include/BALL/VIEW/GUI/WIDGETS/mainControlPreferences.h -o mainControlPreferences_moc.C

scene_moc.C:	$(BALL_PATH)/include/BALL/VIEW/GUI/WIDGETS/scene.h
	$(MOC) $(BALL_PATH)/include/BALL/VIEW/GUI/WIDGETS/scene.h -o scene_moc.C

serverPreferences_moc.C:	$(BALL_PATH)/include/BALL/VIEW/GUI/WIDGETS/serverPreferences.h
	$(MOC) $(BALL_PATH)/include/BALL/VIEW/GUI/WIDGETS/serverPreferences.h -o serverPreferences_moc.C

regularData1DWidget_moc.C:	$(BALL_PATH)/include/BALL/VIEW/GUI/WIDGETS/regularData1DWidget.h
	$(MOC) $(BALL_PATH)/include/BALL/VIEW/GUI/WIDGETS/regularData1DWidget.h -o regularData1DWidget_moc.C

regularData2DWidget_moc.C:	$(BALL_PATH)/include/BALL/VIEW/GUI/WIDGETS/regularData2DWidget.h
	$(MOC) $(BALL_PATH)/include/BALL/VIEW/GUI/WIDGETS/regularData2DWidget.h -o regularData2DWidget_moc.C

if PYTHON_SUPPORT
pyWidget_moc.C:	$(BALL_PATH)/include/BALL/VIEW/GUI/WIDGETS/pyWidget.h
	$(MOC) $(BALL_PATH)/include/BALL/VIEW/GUI/WIDGETS/pyWidget.h -o pyWidget_moc.C
endif

