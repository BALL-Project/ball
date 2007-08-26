# -*- Mode: C++; tab-width: 2; -*-
# vi: set ts=2:
#
# $Id: Makefile,v 1.12.14.2 2007/03/25 23:30:43 amoll Exp $
#
# Author:
#   Oliver Kohlbacher
#

DIRECTORY=VIEW/DATATYPE

# Where the headers of the derived classes reside
HEADER_DIR=$(BALL_PATH)/include/BALL/$(DIRECTORY)

THISLIB=$(LIBNAME)

CPP_SOURCES=\
	colorExtensions.C\
	colorHSV.C\
	colorRGBA.C\
	colorMap.C\
	colorUnit.C\
	colorUnitHue.C\
	vertex1.C\
	vertex2.C\
	dataset.C\
	raytraceableGrid.C\
	standardDatasets.C\

MOC_SOURCES=\
	dataset_moc.C\
	standardDatasets_moc.C\

include ../../common.mak
-include .Dependencies

depend: $(MOC_SOURCES)

dataset_moc.C:	$(HEADER_DIR)/dataset.h
	$(MOC) $(HEADER_DIR)/dataset.h -o $@

standardDatasets_moc.C:	$(HEADER_DIR)/standardDatasets.h
	$(MOC) $(HEADER_DIR)/standardDatasets.h -o $@

