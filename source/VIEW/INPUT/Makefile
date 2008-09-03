# -*- Mode: C++; tab-width: 2; -*-
# vi: set ts=2:
#
# $Id: Makefile,v 1.26.14.2 2007/03/25 23:30:47 amoll Exp $
#
# Author:
#   Oliver Kohlbacher
#

DIRECTORY=VIEW/INPUT

# Where the headers of the derived classes reside
HEADER_DIR=$(BALL_PATH)/include/BALL/$(DIRECTORY)

# Where the UIC-created headers reside
UIC_DIR=$(BALL_PATH)/include/BALL/VIEW/UIC

THISLIB=$(LIBNAME)

CPP_SOURCES=\
	transformationEvent6D.C\
	inputDeviceDriver.C\
	spaceNavigatorDriver.C\

PYTHON_CPP_SOURCES=

MOC_SOURCES=

PYTHON_MOC_SOURCES=

include ../../common.mak
-include .Dependencies

BALL_INCLUDES+=$(PYTHON_INCLUDES)

depend: $(MOC_SOURCES)

