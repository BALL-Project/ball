# $Id: Makefile,v 1.5.4.1 2005/06/19 09:32:46 oliver Exp $

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

