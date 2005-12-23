# $Id: Makefile,v 1.8 2005/12/23 17:02:30 amoll Exp $

EXECUTABLE=nmrview
DIRECTORY=APPLICATIONS/NMRVIEW
all:	$(EXECUTABLE)

clean:	default_clean

# replace library-specific flags by application-
# specific compiler flags
LIB_CXXFLAGS=$(NONLIB_CXXFLAGS)

include ../../common.mak
-include .Dependencies


BALL_INCLUDES=$(VIEW_INCLUDES)

CPP_SOURCES=nmrview.C

MOC_SOURCES=

SPECIAL_OBJECTS=

nmrview:	$(OBJECTS) $(SPECIAL_OBJECTS)
	$(CXX)	$(LDFLAGS) -o $(EXECUTABLE) $(OBJECTS) $(SPECIAL_OBJECTS) $(VIEW_LIBS) $(PYTHON_LIBS)

