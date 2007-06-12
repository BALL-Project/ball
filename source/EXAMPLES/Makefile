# -*- Mode: C++; tab-width: 2; -*-
# vi: set ts=2:
#
# $Id: Makefile,v 1.23.16.2 2007/06/12 19:41:02 oliver Exp $
#
# Author:
#   Oliver Kohlbacher
#

#========================
# BALL/EXAMPLES Makefile
#========================


include ../config.mak

DIRECTORY=EXAMPLES

EXECUTABLES=\
	proteinMapper\
	PDB2discover\
	amberEnergy\
	bindingEnergy\
	ramachandran\
	PDBChecker\
	addHydrogens\
	SASAreas\
	energy

ifeq ($(BALL_HAS_VIEW),true)
	EXECUTABLES += BALLVIEWClient
	CPP_SOURCES += BALLVIEWClient.C
endif

default:	$(EXECUTABLES)

include ../common.mak
-include .Dependencies

# remove the library specific flags
LIB_CXXFLAGS = $(NONLIB_CXXFLAGS)

clean:	default_clean
	@-$(RM) $(EXECUTABLES) *.o 2>/dev/null

proteinMapper:	proteinMapper.o 
	$(CXX)	$(LDFLAGS) -o $@ $@.o $(LIBS)

PDB2discover:	PDB2discover.o 
	$(CXX)	$(LDFLAGS) -o $@ $@.o $(LIBS)

amberEnergy:	amberEnergy.o 
	$(CXX)	$(LDFLAGS) -o $@ $@.o $(LIBS)

energy:	energy.o 
	$(CXX)	$(LDFLAGS) -o $@ $@.o $(LIBS)

bindingEnergy:	bindingEnergy.o 
	$(CXX)	$(LDFLAGS) -o $@ $@.o $(LIBS)

BALLVIEWClient:	BALLVIEWClient.o 
	$(CXX)	$(LDFLAGS) -o $@ $@.o $(BALL_LIBS)

ramachandran:	ramachandran.o 
	$(CXX)	$(LDFLAGS) -o $@ $@.o $(LIBS)

PDBChecker:	PDBChecker.o 
	$(CXX)	$(LDFLAGS) -o $@ $@.o $(LIBS)

addHydrogens:	addHydrogens.o
	$(CXX)	$(LDFLAGS) -o $@ $@.o $(LIBS)

SASAreas:	SASAreas.o 
	$(CXX)	$(LDFLAGS) -o $@ $@.o $(LIBS)
