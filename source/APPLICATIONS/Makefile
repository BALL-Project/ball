# -*- Mode: C++; tab-width: 2; -*-
# vi: set ts=2:
#
# $Id: Makefile,v 1.9.2.2 2006/04/03 09:11:22 amoll Exp $
#
# Author:
#   Oliver Kohlbacher
#

include ../config.mak

DIRECTORY=APPLICATIONS

THISLIB=$(LIBNAME)

OPTIONAL_DIRS=\
	BALLVIEW\
	DCD2PNG
#	NMRVIEW\

VIEW_DIRS=$(BALL_HAS_VIEW:true=$(OPTIONAL_DIRS))

DIRS=$(VIEW_DIRS)\
	PB\
	AMBER\
	UTILITIES


default:	dirs

dirs:	dummy
	@$(MAKE) subdirs SUBDIRS="$(DIRS)" TARGET="all"

depend:	dummy
	@for i in $(DIRS) ; do ( cd $$i ; echo "" >.Dependencies ) ; done
	@$(MAKE) subdirs SUBDIRS="$(DIRS)" TARGET=depend

subdirs:	dummy
	@set -e; for i in $(SUBDIRS); do cd $$i; if $(MAKE) $(TARGET); then cd .. ; else cd .. ; exit 1 ; fi ; done

default_clean:
	@$(MAKE) subdirs SUBDIRS="$(DIRS)" TARGET=clean

collect:
	
dummy:
