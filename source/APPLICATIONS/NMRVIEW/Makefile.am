# $Id: Makefile.am,v 1.1.2.1 2002/12/04 20:00:49 anhi Exp $

EXECUTABLE=nmrview
DIRECTORY=APPLICATIONS/NMRVIEW
all:	$(EXECUTABLE)

clean:	default_clean

# replace library-specific flags by application-
# specific compiler flags
LIB_CXXFLAGS=$(NONLIB_CXXFLAGS)

include ../../common.mak
include .Dependencies


BALL_INCLUDES=$(BALLVIEW_INCLUDES)

CPP_SOURCES=nmrview.C

MOC_SOURCES=

SPECIAL_OBJECTS=

nmrview:	$(OBJECTS) $(SPECIAL_OBJECTS)
	$(CXX)	$(LDFLAGS) -o $(EXECUTABLE) $(OBJECTS) $(SPECIAL_OBJECTS) $(BALLVIEW_LIBS) $(PYTHON_LIBS)

