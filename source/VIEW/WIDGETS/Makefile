# -*- Mode: C++; tab-width: 2; -*-
# vi: set ts=2:
#
# $Id: Makefile,v 1.26.14.2 2007/03/25 23:30:47 amoll Exp $
#
# Author:
#   Oliver Kohlbacher
#

DIRECTORY=VIEW/WIDGETS

# Where the headers of the derived classes reside
HEADER_DIR=$(BALL_PATH)/include/BALL/$(DIRECTORY)

# Where the UIC-created headers reside
UIC_DIR=$(BALL_PATH)/include/BALL/VIEW/UIC

THISLIB=$(LIBNAME)

CPP_SOURCES=\
	dockWidget.C\
	editableScene.C\
	logView.C\
 	genericControl.C\
	geometricControl.C\
	molecularControl.C\
	molecularStructure.C\
	scene.C\
	dockingController.C\
	helpViewer.C\
	fileObserver.C\
	shortcutTableView.C\
	SDWidget.C\
	testFramework.C\
	datasetControl.C

#	canvasWidget.C\
#	regularData2DWidget.C\
#	regularData1DWidget.C\


PYTHON_CPP_SOURCES=\
	pyWidget.C

MOC_SOURCES=\
	datasetControl_moc.C\
	dockWidget_moc.C\
	dockingController_moc.C\
	editableScene_moc.C\
	genericControl_moc.C\
	geometricControl_moc.C\
	helpViewer_moc.C\
	logView_moc.C\
	molecularControl_moc.C\
	molecularStructure_moc.C\
	fileObserver_moc.C\
	testFramework_moc.C\
	shortcutTableView_moc.C\
	scene_moc.C\
	SDWidget_moc.C
#	canvasWidget_moc.C\
#	regularData1DWidget_moc.C\
#	regularData2DWidget_moc.C\
#	stereoHalfImage_moc.C

PYTHON_MOC_SOURCES=\
	pyWidget_moc.C

include ../../common.mak
-include .Dependencies

BALL_INCLUDES+=$(PYTHON_INCLUDES)

depend: $(MOC_SOURCES)

dockWidget_moc.C:	$(HEADER_DIR)/dockWidget.h
	$(MOC) $(HEADER_DIR)/dockWidget.h -o $@

editableScene_moc.C:	$(HEADER_DIR)/editableScene.h
	$(MOC) $(HEADER_DIR)/editableScene.h -o $@

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

shortcutTableView_moc.C: $(HEADER_DIR)/shortcutTableView.h
	$(MOC) $(HEADER_DIR)/shortcutTableView.h -o $@

#stereoHalfImage_moc.C:	$(HEADER_DIR)/stereoHalfImage.h
#	$(MOC) $(HEADER_DIR)/stereoHalfImage.h -o $@

pyWidget_moc.C:	$(HEADER_DIR)/pyWidget.h
	$(MOC) $(HEADER_DIR)/pyWidget.h -o $@

fileObserver_moc.C:	$(HEADER_DIR)/fileObserver.h
	$(MOC) $(HEADER_DIR)/fileObserver.h -o $@

regularData1DWidget_moc.C:	$(HEADER_DIR)/regularData1DWidget.h
	$(MOC) $(HEADER_DIR)/regularData1DWidget.h -o $@

regularData2DWidget_moc.C:	$(HEADER_DIR)/regularData2DWidget.h
	$(MOC) $(HEADER_DIR)/regularData2DWidget.h -o $@

canvasWidget_moc.C:	$(HEADER_DIR)/canvasWidget.h
	$(MOC) $(HEADER_DIR)/canvasWidget.h -o $@

datasetControl_moc.C:	$(HEADER_DIR)/datasetControl.h
	$(MOC) $(HEADER_DIR)/datasetControl.h -o $@

helpViewer_moc.C:	$(HEADER_DIR)/helpViewer.h
	$(MOC) $(HEADER_DIR)/helpViewer.h -o $@

dockingController_moc.C:	$(HEADER_DIR)/dockingController.h
	$(MOC) $(HEADER_DIR)/dockingController.h -o $@

testFramework_moc.C:	$(HEADER_DIR)/testFramework.h
	$(MOC) $(HEADER_DIR)/testFramework.h -o $@

SDWidget_moc.C:	$(HEADER_DIR)/SDWidget.h
	$(MOC) $(HEADER_DIR)/SDWidget.h -o $@
