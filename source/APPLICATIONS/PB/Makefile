# $Id: Makefile,v 1.6 2001/07/31 00:46:49 oliver Exp $
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
