# $Id: Makefile,v 1.22.2.1 2003/01/07 13:21:41 anker Exp $

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
	@touch EXTENSIONS/Makefile.inc
	@cd EXTENSIONS ; $(MAKE) depend
