# $Id: Makefile.am,v 1.1.2.1 2002/12/04 20:00:56 anhi Exp $

include ../Make.rules.in

DIRECTORY=CONCEPT

if BALL_HAS_XDR
ADD_SRC=XDRPersistenceManager.C
else
ADD_SRC=
endif

_objects_LINK = ../config/genobjects.sh $(DIRECTORY)
	
CURRENTDIR_SOURCES=\
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
	$(ADD_SRC)
