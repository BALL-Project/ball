DIRECTORY=VIEW/WIDGETS

# Where the headers of the derived classes reside
HEADER_DIR		=$(BALL_PATH)/include/BALL/$(DIRECTORY)

THISLIB=$(LIBNAME)

CPP_SOURCES=\
	control.C\
	geometricControl.C\
	logView.C\
	molecularControl.C\
	molecularProperties.C\
	regularData1DWidget.C\
	scene.C

PYTHON_CPP_SOURCES=\
	pyWidget.C

MOC_SOURCES=\
	control_moc.C\
	geometricControl_moc.C\
	logView_moc.C\
	molecularControl_moc.C\
	scene_moc.C\
	regularData1DWidget_moc.C

PYTHON_MOC_SOURCES=\
	pyWidget_moc.C

include ../../common.mak
include .Dependencies

BALL_INCLUDES=$(VIEW_INCLUDES) $(PYTHON_INCLUDES)

$(UIC_SOURCES):	$(UIC_HDR)

depend: $(UIC_SOURCES) $(MOC_SOURCES)

control_moc.C:	$(HEADER_DIR)/control.h
	$(MOC) $(HEADER_DIR)/control.h -o $@

geometricControl_moc.C:	$(HEADER_DIR)/geometricControl.h
	$(MOC) $(HEADER_DIR)/geometricControl.h -o $@

logView_moc.C:	$(HEADER_DIR)/logView.h
	$(MOC) $(HEADER_DIR)/logView.h -o $@

molecularControl_moc.C:	$(HEADER_DIR)/molecularControl.h
	$(MOC) $(HEADER_DIR)/molecularControl.h -o $@

scene_moc.C:	$(HEADER_DIR)/scene.h
	$(MOC) $(HEADER_DIR)/scene.h -o $@

pyWidget_moc.C:	$(HEADER_DIR)/pyWidget.h
	$(MOC) $(HEADER_DIR)/pyWidget.h -o $@

regularData1DWidget_moc.C:	$(HEADER_DIR)/regularData1DWidget.h
	$(MOC) $(HEADER_DIR)/regularData1DWidget.h -o $@

regularData2DWidget_moc.C:	$(HEADER_DIR)/regularData2DWidget.h
	$(MOC) $(HEADER_DIR)/regularData2DWidget.h -o $@
