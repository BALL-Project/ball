# -*- Mode: C++; tab-width: 2; -*-
# vi: set ts=2:
#
# $Id: Makefile,v 1.1.2.1 2005/11/02 20:21:36 oliver Exp $
#

DIRECTORY=TEST

# Include the list of test programs.
# This file is also included by the corresponding
# Windows makefile to build the tests under Windows.
# Please remember to include the $(EXEEXT) at the end
# of the executable name!
include Makefile.lst


EXECUTABLES = $(BALL_HAS_XDR:true=$(XDR_DEPENDENT_EXECUTABLES))\
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
	$(EXECUTABLES_SOLVATION)
 
# default target
all:	test

echo:
	echo $(EXECUTABLES)

include ../../../common.mak

BALL_PY_LIBPATH="$(BALL_PATH)/lib/$(BINFMT)"

dummy:
	@echo 'export BALL_PY_LIBPATH="'${BALL_PY_LIBPATH}'"' > .vars
	@echo 'export PYTHON="'${PYTHON_EXECUTABLE}'"' >> .vars

test: info basic_test system_test maths_test datatypes_test concepts_test kernel_test formats_test energy_test molmec_test nmr_test structure_test qsar_test solvation_test
	@./runtests -success

info:
	@./runtests -info

basic_test:	dummy
	@./runtests -section BASIC $(EXECUTABLES_BASIC) $(TEST_OPTIONS) 

system_test:	 dummy
	@./runtests -section SYSTEM $(EXECUTABLES_SYSTEM) $(TEST_OPTIONS) 

maths_test:	 dummy
	@./runtests -section MATHS $(EXECUTABLES_MATHS) $(TEST_OPTIONS) 

datatypes_test:	 dummy
	@./runtests -section DATATYPES $(EXECUTABLES_DATATYPES) $(TEST_OPTIONS) 
	
concepts_test:	 dummy
	@./runtests -section CONCEPTS $(EXECUTABLES_CONCEPTS) $(TEST_OPTIONS) 
	
kernel_test:	 dummy
	@./runtests -section KERNEL $(EXECUTABLES_KERNEL) $(TEST_OPTIONS) 
	
formats_test:	
	@./runtests -section FORMATS $(EXECUTABLES_FORMATS) $(TEST_OPTIONS) 
	
energy_test:	
	@./runtests -section ENERGY $(EXECUTABLES_ENERGY) $(TEST_OPTIONS) 
	
molmec_test:	
	@./runtests -section MOLMEC $(EXECUTABLES_MOLMEC) $(TEST_OPTIONS) 
	
nmr_test:	
	@./runtests -section NMR $(EXECUTABLES_NMR) $(TEST_OPTIONS) 

structure_test:	
	@./runtests -section STRUCTURE $(EXECUTABLES_STRUCTURE) $(TEST_OPTIONS) 

qsar_test:	
	@./runtests -section QSAR $(EXECUTABLES_QSAR) $(TEST_OPTIONS) 

solvation_test:	
	@./runtests -section SOLVATION $(EXECUTABLES_SOLVATION) $(TEST_OPTIONS) 

clean:	default_clean
	@$(RM) -rf *.log *.TMP core core.* 
