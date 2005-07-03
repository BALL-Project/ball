# $Id: Makefile,v 1.25 2005/07/03 09:43:24 oliver Exp $

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
