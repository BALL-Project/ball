# $Id: Makefile,v 1.5 2000/05/05 18:20:13 oliver Exp $
# Makefile for Finite Difference Poisson-Boltzmann Application
#


EXECUTABLE=PB
DIRECTORY=APPLICATIONS/PB

all:	$(EXECUTABLE)

clean:	default_clean

include ../../common.mak
include .Dependencies

SOURCES=\
	main.C\
	global.C\
	reading.C\
	assignment.C

CPP_SOURCES=$(SOURCES)

SPECIAL_OBJECTS=

PB:	$(OBJECTS) $(SPECIAL_OBJECTS)
	$(CXX)	$(LDFLAGS) -o $(EXECUTABLE) $(OBJECTS) $(SPECIAL_OBJECTS) $(BALL_LIBS) -lm
