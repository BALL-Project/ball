DIRECTORY=CONCEPT

THISLIB=$(LIBNAME)

XDR_SOURCES=$(BALL_HAS_XDR:true=XDRPersistenceManager.C)

CPP_SOURCES= $(XDR_SOURCES)\
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
	visitor.C

include ../common.mak
include .Dependencies
