#
# BALL Python support Makefile
#
# $Id: Makefile,v 1.5 2001/12/28 03:31:40 oliver Exp $
#

DIRECTORY=PYTHON_EXTENSIONS

# allow processing of SIP files
.SUFFIXES: .sip

# default target: the BALL Python extensions in libBALLc.so
all:	libBALLc.so

#
include ../../common.mak
-include Makefile.inc

SIP_OPTS=-c SIP -o -e -s ".C" -m "Makefile.inc"
SIPFLAGS += $(PYTHON_INCLUDES) $(SIP_INCLUDES)
CXXFLAGS += $(SIPFLAGS)
 
BALL_INCLUDES+=$(BALLVIEW_INCLUDES)
BALL_LIBS=$(BALLVIEW_LIBS)
ADD_INCLUDES=$(PYTHON_INCLUDES) $(SIP_INCLUDES)
 
PYBALL_SOURCES = \
	extractors.C \
	pyAtomList.C \
	pyBondList.C \
	pyConstant.C \
	pyMoleculeList.C \
	pyNucleotideList.C \
	pyProteinList.C \
	pySecondaryStructureList.C \
	pyAtomContainerList.C \
	pyBALLSipHelper.C \
	pyChainList.C \
	pyFragmentList.C \
	pyNucleicAcidList.C \
	pyPDBAtomList.C \
	pyResidueList.C

PYBALL_OBJECTS  = $(PYBALL_SOURCES:.C=.o)
 
CPP_SOURCES = $(SIP_SOURCES:.h=.C) $(PYBALL_SOURCES)
CPP_OBJECTS = $(SIP_OBJECTS) $(PYBALL_OBJECTS)

BALL.py:	sip
sipBALLVersion.h:	sip_helper
sip_helper.C:	Makefile.inc
pyBALLSipHelper.C: sipBALLVersion.h

sip:
	@if ! test -d SIP ; then mkdir SIP 2>/dev/null ; fi
	@echo "running SIP..."
	@$(SIP) $(SIP_OPTS) BALL.sip
	@echo "removing SIP headers..."
	@for i in SIP/*; do mv $$i SIP/tmp ; cat SIP/tmp |sed 3d > $$i ; rm -f SIP/tmp ; done
	@echo "updating modified files..."
	@for i in SIP/*; do if ! cmp -s $$i `basename $$i`; then echo " - $$i"; cp $$i . ; fi ; done


libBALLc.so:  sipBALLVersion.h $(CPP_OBJECTS)
	$(DYNAR) $(DYNAROPTS) libBALLc.so $(SIP_OBJECTS) $(PYBALL_OBJECTS) $(SIP_LIB) -L$(BALL_PATH)/lib/$(BINFMT) $(BALL_LIBS)

install:	libBALLc.so
	cp libBALLc.so BALL.py $(BALL_PATH)/lib/$(BINFMT)
	

			 	
 
CLEAN_FILES = \
	BALL.pyc \
	BALL.py \
	sipBALLVersion.h \
	sip_helper.C \
	sip_helper \
	$(CPP_OBJECTS) \
	$(SIP_SOURCES) \
	$(SIP_HEADERS) \
	$(SIP_OBJECTS)
 
clean:
	@echo "removing object files, libraries, and source/header files produced by SIP..."
	@-$(RM) $(CLEAN_FILES) 2>/dev/null


mydepend: sipBALLVersion.h depend Makefile.inc .Dependencies

-include .Dependencies
