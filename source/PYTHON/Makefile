# $Id: Makefile,v 1.23 2002/02/15 01:57:27 oliver Exp $

DIRECTORY=PYTHON

THISLIB=$(LIBNAME)

CPP_SOURCES=\
	pyInterpreter.C

include ../common.mak
include .Dependencies

ADD_INCLUDES=$(PYTHON_INCLUDES)

depend: subdepend

subdepend:
	@touch EXTENSIONS/.Dependencies
	@cd EXTENSIONS ; $(MAKE) depend
