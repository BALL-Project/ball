#
# $Id: Makefile,v 1.2 2003/08/26 09:18:35 oliver Exp $
#

include ../../config.mak

DIRECTORY=VIEW/GUI

THISLIB=$(LIBNAME)

DIRS=\
	DIALOGS\
	FUNCTOR\
	KERNEL\
	WIDGETS

default: dirs

dirs:		dummy
	@$(MAKE) subdirs SUBDIRS="$(DIRS)" TARGET=default

collectlib:	dummy
	@$(MAKE) subdirs SUBDIRS="$(DIRS)" TARGET=collectlib LIBNAME="$(THISLIB)"

depend:	dummy
	@for i in $(DIRS) ; do ( cd $$i; echo "" >.Dependencies; cd .. ) ; done
	@$(MAKE) subdirs SUBDIRS="$(DIRS)" TARGET=depend

subdirs:	dummy
	@set -e; for i in $(SUBDIRS); do cd $$i; if $(MAKE) $(TARGET); then cd .. ; else cd .. ; exit 1 ; fi ; done

default_clean:
	@$(MAKE) subdirs SUBDIRS="$(DIRS)" TARGET=default_clean


collect:	$(OBJECT_LISTS)
	
dummy:
