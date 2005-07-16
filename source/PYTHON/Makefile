#
# $Id: Makefile,v 1.26 2005/07/16 21:00:42 oliver Exp $
#

DIRECTORY=PYTHON

THISLIB=$(LIBNAME)

CPP_SOURCES=\
	pyInterpreter.C

include ../common.mak
-include .Dependencies

ADD_INCLUDES=$(PYTHON_INCLUDES)

depend: subdepend

subdepend:
	@touch EXTENSIONS/.Dependencies
	@touch EXTENSIONS/Makefile.inc
	@cd EXTENSIONS ; $(MAKE) depend
