DIRECTORY=VIEW/WIDGETS

# Where the headers of the derived classes reside
HEADER_DIR=$(BALL_PATH)/include/BALL/$(DIRECTORY)

# Where the UIC-created headers reside
UIC_DIR=$(BALL_PATH)/include/BALL/VIEW/UIC

THISLIB=$(LIBNAME)

CPP_SOURCES=\
	dockWidget.C\
	genericControl.C\
	logView.C\
	geometricControl.C\
	datasetControl.C\
	molecularControl.C\
	molecularStructure.C\
	scene.C\
	canvasWidget.C\
	regularData2DWidget.C\
	regularData1DWidget.C


PYTHON_CPP_SOURCES=\
	pyWidget.C

MOC_SOURCES=\
	dockWidget_moc.C\
	genericControl_moc.C\
	logView_moc.C\
	datasetControl_moc.C\
	geometricControl_moc.C\
	molecularControl_moc.C\
	molecularStructure_moc.C\
	scene_moc.C\
	canvasWidget_moc.C
	regularData1DWidget_moc.C\
	regularData2DWidget_moc.C

PYTHON_MOC_SOURCES=\
	pyWidget_moc.C

include ../../common.mak
include .Dependencies

BALL_INCLUDES=$(VIEW_INCLUDES) $(PYTHON_INCLUDES)

depend: $(MOC_SOURCES)

dockWidget_moc.C:	$(HEADER_DIR)/dockWidget.h
	$(MOC) $(HEADER_DIR)/dockWidget.h -o $@

genericControl_moc.C:	$(HEADER_DIR)/genericControl.h
	$(MOC) $(HEADER_DIR)/genericControl.h -o $@

geometricControl_moc.C:	$(HEADER_DIR)/geometricControl.h
	$(MOC) $(HEADER_DIR)/geometricControl.h -o $@

logView_moc.C:	$(HEADER_DIR)/logView.h
	$(MOC) $(HEADER_DIR)/logView.h -o $@

molecularControl_moc.C:	$(HEADER_DIR)/molecularControl.h
	$(MOC) $(HEADER_DIR)/molecularControl.h -o $@

molecularStructure_moc.C:	$(HEADER_DIR)/molecularStructure.h
	$(MOC) $(HEADER_DIR)/molecularStructure.h -o $@

scene_moc.C:	$(HEADER_DIR)/scene.h
	$(MOC) $(HEADER_DIR)/scene.h -o $@

pyWidget_moc.C:	$(HEADER_DIR)/pyWidget.h
	$(MOC) $(HEADER_DIR)/pyWidget.h -o $@

regularData1DWidget_moc.C:	$(HEADER_DIR)/regularData1DWidget.h
	$(MOC) $(HEADER_DIR)/regularData1DWidget.h -o $@

regularData2DWidget_moc.C:	$(HEADER_DIR)/regularData2DWidget.h
	$(MOC) $(HEADER_DIR)/regularData2DWidget.h -o $@

canvasWidget_moc.C:	$(HEADER_DIR)/canvasWidget.h
	$(MOC) $(HEADER_DIR)/canvasWidget.h -o $@

datasetControl_moc.C:	$(HEADER_DIR)/datasetControl.h
	$(MOC) $(HEADER_DIR)/datasetControl.h -o $@

