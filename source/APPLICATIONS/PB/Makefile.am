# $Id: Makefile.am,v 1.1.2.1 2002/12/04 20:00:51 anhi Exp $
# Makefile for Finite Difference Poisson-Boltzmann Application
#


EXECUTABLE=PB
DIRECTORY=APPLICATIONS/PB

all:	$(EXECUTABLE)

clean:	default_clean

include ../../common.mak
include .Dependencies

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
	$(CXX)	$(LDFLAGS) -o $(EXECUTABLE) $(OBJECTS) $(SPECIAL_OBJECTS) $(BALL_LIBS) -lm
