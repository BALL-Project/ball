# MOLMEC/Makefile
# $Id: Makefile,v 1.5 2000/02/10 11:55:45 oliver Exp $
#
include ../config.mak

DIRECTORY=MOLMEC

THISLIB=$(LIBNAME)

DIRS=\
	AMBER\
	CHARMM\
	COMMON\
	MINIMIZATION\
	MDSIMULATION\
	PARAMETER

default:	dirs

dirs:	dummy
	@$(MAKE) subdirs SUBDIRS="$(DIRS)" TARGET="default"

collectlib:	dummy
	@$(MAKE) subdirs SUBDIRS="$(DIRS)" TARGET=collectlib LIBNAME="$(THISLIB)"

depend:	dummy
	@for i in $(DIRS) ; do ( cd $$i ; echo "" >.Dependencies ) ; done
	@$(MAKE) subdirs SUBDIRS="$(DIRS)" TARGET=depend

subdirs:	dummy
	@set -e; for i in $(SUBDIRS); do cd $$i; if $(MAKE) $(TARGET); then cd .. ; else cd .. ; exit 1 ; fi ; done

default_clean:
	@$(MAKE) subdirs SUBDIRS="$(DIRS)" TARGET=default_clean


collect:	$(OBJECT_LISTS)
	
dummy:
