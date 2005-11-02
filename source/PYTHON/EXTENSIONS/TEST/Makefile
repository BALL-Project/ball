# -*- Mode: C++; tab-width: 2; -*-
# vi: set ts=2:
#
# $Id: Makefile,v 1.1.2.2 2005/11/02 21:29:26 oliver Exp $
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

test: dummy info basic_test system_test maths_test datatypes_test concepts_test kernel_test formats_test energy_test molmec_test nmr_test structure_test qsar_test solvation_test
	@./runtests -success

info:
	@./runtests -info

basic_test:	dummy
	@./runtests -section BASIC  $(TEST_OPTIONS) $(EXECUTABLES_BASIC)

system_test:	 dummy
	@./runtests -section SYSTEM  $(TEST_OPTIONS) $(EXECUTABLES_SYSTEM)

maths_test:	 dummy
	@./runtests -section MATHS  $(TEST_OPTIONS) $(EXECUTABLES_MATHS)

datatypes_test:	 dummy
	@./runtests -section DATATYPES  $(TEST_OPTIONS) $(EXECUTABLES_DATATYPES)
	
concepts_test:	 dummy
	@./runtests -section CONCEPTS $(TEST_OPTIONS) $(EXECUTABLES_CONCEPTS)
	
kernel_test:	 dummy
	@./runtests -section KERNEL $(TEST_OPTIONS) $(EXECUTABLES_KERNEL)
	
formats_test:	
	@./runtests -section FORMATS $(TEST_OPTIONS) $(EXECUTABLES_FORMATS)
	
energy_test:	
	@./runtests -section ENERGY  $(TEST_OPTIONS) $(EXECUTABLES_ENERGY)
	
molmec_test:	
	@./runtests -section MOLMEC $(TEST_OPTIONS) $(EXECUTABLES_MOLMEC)
	
nmr_test:	
	@./runtests -section NMR $(TEST_OPTIONS) $(EXECUTABLES_NMR)

structure_test:	
	@./runtests -section STRUCTURE $(TEST_OPTIONS) $(EXECUTABLES_STRUCTURE)

qsar_test:	
	@./runtests -section QSAR $(TEST_OPTIONS) $(EXECUTABLES_QSAR)

solvation_test:	
	@./runtests -section SOLVATION $(TEST_OPTIONS) $(EXECUTABLES_SOLVATION)

clean:	default_clean
	@$(RM) -rf *.log *.TMP core core.* 
