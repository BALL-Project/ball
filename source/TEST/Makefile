# -*- Mode: C++; tab-width: 2; -*-
# vi: set ts=2:
#
# $Id: Makefile,v 1.211 2003/08/26 09:18:30 oliver Exp $
#

DIRECTORY=TEST

# Include the list of test programs.
# This file is also included by the corresponding
# Windows makefile to build the tests under Windows.
# Please remember to include the $(EXEEXT) at the end
# of the executable name!
include Makefile.lst

EXECUTABLES_VIEW = \
	PrimitiveManager_test\
	Stage_test

EXECUTABLES = \
	$(EXECUTABLES_BASIC)\
	$(EXECUTABLES_SYSTEM)\
	$(EXECUTABLES_MATHS)\
	$(EXECUTABLES_DATATYPES)\
	$(EXECUTABLES_CONCEPTS)\
	$(EXECUTABLES_KERNEL)\
	$(EXECUTABLES_FORMATS)\
	$(EXECUTABLES_MOLMEC)\
	$(EXECUTABLES_NMR)\
	$(EXECUTABLES_ENERGY)\
	$(EXECUTABLES_STRUCTURE)\
	$(EXECUTABLES_QSAR)\
	$(EXECUTABLES_SOLVATION)\
	$(BALL_HAS_XDR:true=$(XDR_DEPENDENT_EXECUTABLES))\
	$(BALL_HAS_VIEW:true=$(VIEW_DEPENDENT_EXECUTABLES))
 

# default target
all:	$(EXECUTABLES)

echo:
	echo $(EXECUTABLES)

# required for clean and depend
CPP_SOURCES=$(EXECUTABLES:=.C)
OBJECTS=$(EXECUTABLES:=.o)
VALGRINDS=$(EXECUTABLES:=.vgr)

include ../common.mak
include .Dependencies

# remove the library specific flags
# and replace them by the aplication specific
# flags
LIB_CXXFLAGS=$(NONLIB_CXXFLAGS)

# do not optimize tests (reduces compile time drastically)
CPP_MODE_FLAGS=$(CPP_MODE_FLAGS_NO_OPTIMIZATION) -O0

.o:
	$(CXX) $(CXXFLAGS) $*.o $(LDFLAGS) -o $@ $(LIBS)
# 
%%.C:
	$(CXX) $(CXXFLAGS) $(ADD_CXXFLAGS) $(CPP_MODE_FLAGS) $(BALL_CXXFLAGS) $(BALL_INCLUDES) $*.C $(LDFLAGS) -o $@ $(LIBS)

.o.vgr: 
	@$(CXX) $(CXXFLAGS) $(ADD_CXXFLAGS) $(CPP_MODE_FLAGS) $(BALL_CXXFLAGS) $(BALL_INCLUDES) $*.o $(LDFLAGS) -o $@ $(LIBS)
	$(VALGRIND) $(VALGRIND_OPTS) $* 2>$*.vgr || true

dummy:

test: info basic_test system_test maths_test datatypes_test concepts_test kernel_test formats_test energy_test molmec_test nmr_test structure_test qsar_test solvation_test
	@./runtests -success

info:
	@./runtests -info

basic_test:	$(EXECUTABLES_BASIC) dummy
	@./runtests -section BASIC $(TEST_OPTIONS) $(EXECUTABLES_BASIC)

system_test:	$(EXECUTABLES_SYSTEM) dummy
	@./runtests -section SYSTEM $(TEST_OPTIONS) $(EXECUTABLES_SYSTEM)

maths_test:	$(EXECUTABLES_MATHS) dummy
	@./runtests -section MATHS $(TEST_OPTIONS) $(EXECUTABLES_MATHS)

datatypes_test:	$(EXECUTABLES_DATATYPES) dummy
	@./runtests -section DATATYPES $(TEST_OPTIONS) $(EXECUTABLES_DATATYPES)
	
concepts_test:	$(EXECUTABLES_CONCEPTS) dummy
	@./runtests -section CONCEPTS $(TEST_OPTIONS) $(EXECUTABLES_CONCEPTS)
	
kernel_test:	$(EXECUTABLES_KERNEL) dummy
	@./runtests -section KERNEL $(TEST_OPTIONS) $(EXECUTABLES_KERNEL)
	
formats_test:	$(EXECUTABLES_FORMATS)
	@./runtests -section FORMATS $(TEST_OPTIONS) $(EXECUTABLES_FORMATS)
	
energy_test:	$(EXECUTABLES_ENERGY)
	@./runtests -section ENERGY $(TEST_OPTIONS) $(EXECUTABLES_ENERGY)
	
molmec_test:	$(EXECUTABLES_MOLMEC)
	@./runtests -section MOLMEC $(TEST_OPTIONS) $(EXECUTABLES_MOLMEC)
	
nmr_test:	$(EXECUTABLES_NMR)
	@./runtests -section NMR $(TEST_OPTIONS) $(EXECUTABLES_NMR)

structure_test:	$(EXECUTABLES_STRUCTURE)
	@./runtests -section STRUCTURE $(TEST_OPTIONS) $(EXECUTABLES_STRUCTURE)

qsar_test:	$(EXECUTABLES_QSAR)
	@./runtests -section QSAR $(TEST_OPTIONS) $(EXECUTABLES_QSAR)

solvation_test:	$(EXECUTABLES_SOLVATION)
	@./runtests -section SOLVATION $(TEST_OPTIONS) $(EXECUTABLES_SOLVATION)

view_test:	$(EXECUTABLES_VIEW)
	@./runtests -section "VIEW" $(TEST_OPTIONS) $(EXECUTABLES_VIEW)

clean:	default_clean
	@$(RM) -rf *.o *~ *.vgr *.log ii_files Templates.DB	SunWS_cache *.TMP core core.* a.out $(EXECUTABLES)

valgrind: $(VALGRINDS)
