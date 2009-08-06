#
# $Id: Makefile,v 1.26.20.2 2007/06/05 08:11:26 oliver Exp $
#

DIRECTORY=PYTHON

THISLIB=$(LIBNAME)

CPP_SOURCES=\
	pyInterpreter.C

include ../common.mak
-include .Dependencies

#A workaround for sip 4.8
CPP_MODE_FLAGS=${CPP_MODE_FLAGS/-pedantic/}

ADD_INCLUDES=$(PYTHON_INCLUDES)

depend: subdepend

ext:
	@cd EXTENSIONS ;  $(MAKE) 

subdepend:
	@touch EXTENSIONS/.Dependencies
	@touch EXTENSIONS/Makefile.inc
	@cd EXTENSIONS ; $(MAKE) depend
