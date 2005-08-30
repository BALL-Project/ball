# 
# $Id: Makefile,v 1.7.4.1 2005/07/21 12:23:36 oliver Exp $
#
#
# APPLICATIONS/Makefile
#
#

include ../config.mak

DIRECTORY=APPLICATIONS

THISLIB=$(LIBNAME)

OPTIONAL_DIRS=\
	BALLVIEW\
	NMRVIEW

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
