# $Id: Makefile,v 1.2 2001/05/22 15:44:57 oliver Exp $

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

