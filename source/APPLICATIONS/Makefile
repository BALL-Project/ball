# 
# $Id: Makefile,v 1.7 2004/05/27 18:13:07 oliver Exp $
#
#
# APPLICATIONS/Makefile
#
#

include ../config.mak

DIRECTORY=APPLICATIONS

THISLIB=$(LIBNAME)

OPTIONAL_DIRS=\
	BALLVIEW

VIEW_DIRS=$(BALL_HAS_VIEW:true=$(OPTIONAL_DIRS))

DIRS=$(VIEW_DIRS)\
	PB\
	AMBER\
	UTILITIES


default:	dirs

dirs:	dummy
	@$(MAKE) subdirs SUBDIRS="$(DIRS)" TARGET="default"

depend:	dummy
	@for i in $(DIRS) ; do ( cd $$i ; echo "" >.Dependencies ) ; done
	@$(MAKE) subdirs SUBDIRS="$(DIRS)" TARGET=depend

subdirs:	dummy
	@set -e; for i in $(SUBDIRS); do cd $$i; if $(MAKE) $(TARGET); then cd .. ; else cd .. ; exit 1 ; fi ; done

default_clean:
	@$(MAKE) subdirs SUBDIRS="$(DIRS)" TARGET=clean

collect:
	
dummy:
