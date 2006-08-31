# -*- Mode: C++; tab-width: 2; -*-
# vi: set ts=2:
#
# $Id: Makefile,v 1.14.2.2.2.1 2006/08/31 14:05:11 leonhardt Exp $
#
# Author:
#   Oliver Kohlbacher
#

DIRECTORY=APPLICATIONS/UTILITIES

include ../../common.mak

default: all



EXECUTABLES = $(FFT_SOURCES)\
	add_hydrogens\
#assign_charges_from_rules\
#assign_radii_from_rules\
	assign_typenames_from_rules\
	calculate_RMSD\
	clip_protein_around_ligand\
	compute_docking_RMSD\
	create_random_numbers\
	dcd2dcd\
	export_fragment\
	hin2mol2\
	pdb2amber_naming\
	pdb2dcd\
	pdb2hin\
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
