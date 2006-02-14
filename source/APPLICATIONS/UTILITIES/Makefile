# -*- Mode: C++; tab-width: 2; -*-
# vi: set ts=2:
#
# $Id: Makefile,v 1.9.6.3 2006/02/14 15:02:06 amoll Exp $
#
# Author:
#   Oliver Kohlbacher
#

DIRECTORY=APPLICATIONS/UTILITIES

include ../../common.mak

default: all



EXECUTABLES = $(FFT_SOURCES)\
	add_hydrogens\
	assign_typenames_from_rules\
	clip_protein_around_ligand\
	compute_docking_RMSD\
	create_random_numbers\
	dcd2dcd\
	export_fragment\
	pdb2amber_naming\
	pdb2dcd\
	reconstruct_fragment\
	solvent_accessibility

ifeq ($(BALL_HAS_FFTW),true)
	EXECUTABLES+=rigid_docking
endif

# default target
all:	$(EXECUTABLES)

# required for clean and depend
CPP_SOURCES=$(EXECUTABLES:=.C)
OBJECTS=$(EXECUTABLES:=.o)


-include .Dependencies

# remove the library specific flags
# and replace them by the application specific
# flags
LIB_CXXFLAGS=$(NONLIB_CXXFLAGS)

.o:
	$(CXX) $(CXXFLAGS) $*.o $(LDFLAGS) -o $@ $(LIBS)

%%.C:
	$(CXX) $(CXXFLAGS) $(ADD_CXXFLAGS) $(CPP_MODE_FLAGS) $(BALL_CXXFLAGS) $(BALL_INCLUDES) $*.C $(LDFLAGS) -o $@ $(LIBS)

clean:  default_clean
	@$(RM) -rf *.o *~ *.log ii_files Templates.DB   SunWS_cache _*.TMP core a.out $(EXECUTABLES)
