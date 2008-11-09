# -*- Mode: C++; tab-width: 2; -*-
# vi: set ts=2:
#
# $Id: Makefile,v 1.5.14.1 2007/03/25 21:32:14 oliver Exp $
#
# Author:
#   Oliver Kohlbacher
#

DIRECTORY=APPLICATIONS/AMBER

EXECUTABLE=amber

all:	$(EXECUTABLE)

clean:	default_clean

include ../../common.mak
-include .Dependencies

SOURCES=\
	main.C\
	global.C\
	files.C

CPP_SOURCES=$(SOURCES)

# replace library-specific flags by application-
# specific compiler flags
LIB_CXXFLAGS=$(NONLIB_CXXFLAGS)

SPECIAL_OBJECTS=

amber:	$(OBJECTS) $(SPECIAL_OBJECTS)
	$(LINKER)	$(LDFLAGS) -L$(PREFIX)/$(LIBDIR) -L$(BALL_PATH)/$(LIBDIR) -o $(EXECUTABLE) $(OBJECTS) $(SPECIAL_OBJECTS) $(BALL_LIBS) $(PYTHON_LIBS)
