# -*- Mode: C++; tab-width: 2; -*-
# vi: set ts=2:
#
# $Id: Makefile,v 1.9 2006/01/26 16:12:53 oliver Exp $
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

