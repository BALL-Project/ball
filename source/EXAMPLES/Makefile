# -*- Mode: C++; tab-width: 2; -*-
# vi: set ts=2:
#
# $Id: Makefile,v 1.23.2.1 2006/02/02 11:42:13 oliver Exp $
#
# Author:
#   Oliver Kohlbacher
#

#========================
# BALL/EXAMPLES Makefile
#========================


include ../config.mak

DIRECTORY=EXAMPLES

CPP_SOURCES=\
	proteinMapper.C\
	PDB2discover.C\
	amberEnergy.C\
	bindingEnergy.C\
	ramachandran.C\
	PDBChecker.C\
	addHydrogens.C\
	SASAreas.C\
	energy.C


ifeq ($(BALL_HAS_VIEW),true)
	EXECUTABLES += BALLViewClient
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
	$(CXX)	$(LDFLAGS) -o $@ $@.o $(VIEW_LIBS)

ramachandran:	ramachandran.o 
	$(CXX)	$(LDFLAGS) -o $@ $@.o $(LIBS)

PDBChecker:	PDBChecker.o 
	$(CXX)	$(LDFLAGS) -o $@ $@.o $(LIBS)

addHydrogens:	addHydrogens.o
	$(CXX)	$(LDFLAGS) -o $@ $@.o $(LIBS)

SASAreas:	SASAreas.o 
	$(CXX)	$(LDFLAGS) -o $@ $@.o $(LIBS)
