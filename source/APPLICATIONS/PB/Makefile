# -*- Mode: C++; tab-width: 2; -*-
# vi: set ts=2:
#
# $Id: Makefile,v 1.9.14.1 2007/03/25 21:32:25 oliver Exp $
#
# Author:
#   Oliver Kohlbacher
#

DIRECTORY=APPLICATIONS/PB

EXECUTABLE=PB

all:	$(EXECUTABLE)

clean:	default_clean

include ../../common.mak
-include .Dependencies

# remove the library specific flags
# and replace them by the aplication specific
# flags
LIB_CXXFLAGS=$(NONLIB_CXXFLAGS)

CPP_SOURCES=\
	main.C\
	global.C\
	reading.C\
	assignment.C

SPECIAL_OBJECTS=

PB:	$(OBJECTS) $(SPECIAL_OBJECTS)
	$(LINKER)	$(LDFLAGS) -L$(PREFIX)/lib -L$(BALL_PATH)/lib -o $(EXECUTABLE) $(OBJECTS) $(SPECIAL_OBJECTS) $(BALL_LIBS) $(PYTHON_LIBS)
