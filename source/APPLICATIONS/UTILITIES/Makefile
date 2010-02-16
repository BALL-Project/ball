# -*- Mode: C++; tab-width: 2; -*-
# vi: set ts=2:
#
# $Id: Makefile,v 1.16.2.3 2006/03/02 18:52:58 anhi Exp $
#
# Author:
#   Oliver Kohlbacher
#

DIRECTORY=APPLICATIONS/UTILITIES

include ../../common.mak

default: all

EXECUTABLES = $(FFT_SOURCES)\
	add_hydrogens\
	assign_charges_from_rules\
	assign_radii_from_rules\
	assign_typenames_from_rules\
	calculate_RMSD\
	clip_protein_around_ligand\
	compute_connected_components\
	compute_docking_RMSD\
	create_random_numbers\
	dcd2dcd\
	atomtyper\
	export_fragment\
	hin2mol2\
	pdb2amber_naming\
	pdb2dcd\
	pdb2hin\
	reconstruct_fragment\
	solvent_accessibility

ifeq ($(BALL_HAS_FFTW),true)
	EXECUTABLES+=rigid_docking 
	
ifeq ($(BALL_HAS_MPI),true)
	EXECUTABLES+=geometricFit_slave
endif
endif

# default target
all:	$(EXECUTABLES)

# required for clean and depend
CPP_SOURCES=$(EXECUTABLES:=.C)
OBJECTS=$(EXECUTABLES:=.$(OBJECT_EXTENSION))


-include .Dependencies

# remove the library specific flags
# and replace them by the application specific
# flags
LIB_CXXFLAGS=$(NONLIB_CXXFLAGS)

%: %.$(OBJECT_EXTENSION)
	$(LINKER) $(CXXFLAGS) $*.$(OBJECT_EXTENSION) $(LDFLAGS) -o $@ $(LIBS)

clean:  default_clean
	@$(RM) -rf *.o *.lo .libs/* *~ *.vgr *.log ii_files Templates.DB	SunWS_cache *.TMP core core.* a.out $(EXECUTABLES)
