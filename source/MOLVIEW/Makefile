include ../config.mak

DIRECTORY=MOLVIEW

THISLIB=libMOLVIEW

DIRS=\
	COMMON\
	FUNCTOR\
	GUI\
	KERNEL\
	PRIMITIV

default: dirs

dirs:		dummy
	@$(MAKE) subdirs SUBDIRS="$(DIRS)" TARGET=default

collectlib:	dummy
	@$(MAKE) subdirs SUBDIRS="$(DIRS)" TARGET=collectlib LIBNAME="$(THISLIB)"

depend:	dummy
	@echo "" > .Dependencies
	@for i in $(DIRS) ; do ( cd $$i; echo "" >.Dependencies; cd .. ) ; done
	@$(MAKE) subdirs SUBDIRS="$(DIRS)" TARGET=depend

subdirs:	dummy
	@set -e; for i in $(SUBDIRS); do cd $$i; if $(MAKE) $(TARGET); then cd .. ; else cd .. ; exit 1 ; fi ; done

default_clean:
	@$(MAKE) subdirs SUBDIRS="$(DIRS)" TARGET=default_clean


collect:	$(OBJECT_LISTS)
	
dummy:
