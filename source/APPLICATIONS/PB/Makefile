# $Id: Makefile,v 1.2 2000/05/05 07:24:29 oliver Exp $
# Makefile for Finite Difference Poisson-Boltzmann Application
#


EXECUTABLE=PB
DIRECTORY=APPLICATIONS/PB

all:	$(EXECUTABLE)

clean:	default_clean

include ../../common.mak
include .Dependencies

SOURCES=\
	PB.C

CPP_SOURCES=$(SOURCES)

SPECIAL_OBJECTS=

PB:	$(OBJECTS) $(SPECIAL_OBJECTS)
	$(CXX)	$(LDFLAGS) -o $(EXECUTABLE) $(OBJECTS) $(SPECIAL_OBJECTS) $(BALLVIEW_LIBS)
