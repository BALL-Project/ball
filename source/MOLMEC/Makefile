# MOLMEC/Makefile
# $Id: Makefile,v 1.5.2.1 2002/02/14 17:02:50 anker Exp $
#
include ../config.mak

DIRECTORY=MOLMEC

THISLIB=$(LIBNAME)

DIRS=\
	AMBER\
	CHARMM\
	COMMON\
	FRESNO\
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
