# $Id: Makefile.am,v 1.1.2.1 2002/12/04 20:00:46 anhi Exp $
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
