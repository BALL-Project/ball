# 
# $Id: Makefile,v 1.2 2003/08/26 09:17:35 oliver Exp $
#
#
# APPLICATIONS/Makefile
#
#

include ../config.mak

DIRECTORY=APPLICATIONS

THISLIB=$(LIBNAME)

DIRS=\
	MOLVIEW\
	NMRVIEW\
	PB\
	AMBER\
	UTILS


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
