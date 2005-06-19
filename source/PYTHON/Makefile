# $Id: Makefile,v 1.24.10.1 2005/06/19 09:32:49 oliver Exp $

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
