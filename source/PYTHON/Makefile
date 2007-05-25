#
# $Id: Makefile,v 1.26.20.1 2007/05/25 10:07:55 oliver Exp $
#

DIRECTORY=PYTHON

THISLIB=$(LIBNAME)

CPP_SOURCES=\
	pyInterpreter.C

default: ext

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
