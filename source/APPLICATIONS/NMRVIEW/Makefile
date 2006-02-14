# -*- Mode: C++; tab-width: 2; -*-
# vi: set ts=2:
#
# $Id: Makefile,v 1.5.6.1 2006/02/14 15:02:05 amoll Exp $
#
# Author:
#   Oliver Kohlbacher
#

EXECUTABLE=nmrview
DIRECTORY=APPLICATIONS/NMRVIEW

all:	$(EXECUTABLE)

clean:	default_clean

# replace library-specific flags by application-
# specific compiler flags
LIB_CXXFLAGS=$(NONLIB_CXXFLAGS)

include ../../common.mak
-include .Dependencies


CPP_SOURCES=\
	nmrview.C

MOC_SOURCES=

SPECIAL_OBJECTS=

nmrview:	$(OBJECTS) $(SPECIAL_OBJECTS)
	$(CXX)	$(LDFLAGS) -o $(EXECUTABLE) $(OBJECTS) $(SPECIAL_OBJECTS) $(VIEW_LIBS) $(PYTHON_LIBS)

