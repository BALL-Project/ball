#
# BALL Python support Makefile
#
# $Id: Makefile,v 1.2 2001/09/03 20:11:09 oliver Exp $
#

DIRECTORY=PYTHON_EXTENSIONS

# allow processing of SIP files
.SUFFIXES: .sip

# default target: the BALL Python extensions in libBALLc.so
all:	libBALLc.so

#
include ../../common.mak
-include Makefile.inc

SIP_OPTS=-c . -o -e -s ".C" -m "Makefile.inc"
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
	@echo "running SIP..."
	@$(SIP) $(SIP_OPTS) BALL.sip



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
	$(SIP_SOURCES) \
	$(SIP_HEADERS) \
	$(SIP_OBJECTS)
 
clean:
	@echo "removing object files, libraries, and source/header files produced by SIP..."
	@-$(RM) $(CLEAN_FILES) 2>/dev/null


mydepend: depend Makefile.inc .Dependencies

-include .Dependencies
