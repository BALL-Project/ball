# $Id: Makefile,v 1.2 2001/05/22 15:44:54 oliver Exp $
# Makefile for AMBER Application
#


EXECUTABLE=amber
DIRECTORY=APPLICATIONS/AMBER

all:	$(EXECUTABLE)

clean:	default_clean

include ../../common.mak
include .Dependencies

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
	$(CXX)	$(LDFLAGS) -o $(EXECUTABLE) $(OBJECTS) $(SPECIAL_OBJECTS) $(BALL_LIBS) -lm
