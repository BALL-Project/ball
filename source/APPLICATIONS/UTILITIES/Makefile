#
# $Id: Makefile,v 1.9.2.1 2005/02/14 16:24:44 anhi Exp $
# Makefile for BALL utilities
#

DIRECTORY=APPLICATIONS/UTILITIES

EXECUTABLES = \
	add_hydrogens\
	assign_typenames_from_rules\
	clip_protein_around_ligand\
	dcd2dcd\
	export_fragment\
	pdb2amber_naming\
	pdb2dcd\
	reconstruct_fragment\
	solvent_accessibility\
	rigid_docking

# default target
all:	$(EXECUTABLES)

# required for clean and depend
CPP_SOURCES=$(EXECUTABLES:=.C)
OBJECTS=$(EXECUTABLES:=.o)

include ../../common.mak
include .Dependencies

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
