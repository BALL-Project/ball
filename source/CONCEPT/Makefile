DIRECTORY=CONCEPT

THISLIB=$(LIBNAME)

#XDR_SOURCES:sh=if test $(BALL_HAS_XDR) != true ; do echo "" ; else echo "XDRPersistenceManager.C" ; fi

XDR_SOURCES=$(BALL_HAS_XDR:true=XDRPersistenceManager.C)

CPP_SOURCES=\
	autoDeletable.C\
	composite.C\
	embeddable.C\
	enumerator.C\
	notification.C\
	object.C\
	persistenceManager.C\
	processor.C\
	property.C\
	selectable.C\
	textPersistenceManager.C\
	timeStamp.C\
	visitor.C\
	$(XDR_SOURCES)



include ../common.mak
include .Dependencies
