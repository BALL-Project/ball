#
# $Id: Makefile,v 1.26.20.2 2007/06/05 08:11:26 oliver Exp $
#

DIRECTORY=PYTHON

THISLIB=$(LIBNAME)

CPP_SOURCES=\
	pyInterpreter.C

ext:
	@cd EXTENSIONS ;  $(MAKE) 

include ../common.mak
-include .Dependencies

ADD_INCLUDES=$(PYTHON_INCLUDES)

depend: subdepend

subdepend:
	@touch EXTENSIONS/.Dependencies
	@touch EXTENSIONS/Makefile.inc
	@cd EXTENSIONS ; $(MAKE) depend
