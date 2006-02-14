# -*- Mode: C++; tab-width: 2; -*-
# vi: set ts=2:
#
# $Id: Makefile,v 1.17.8.1 2006/02/14 15:02:09 amoll Exp $
#
# Author:
#   Oliver Kohlbacher
#

include ../config.mak

DIRECTORY=CONCEPT

THISLIB=$(LIBNAME)

CPP_SOURCES=\
	autoDeletable.C\
	composite.C\
	embeddable.C\
	enumerator.C\
	factory.C\
	object.C\
	persistenceManager.C\
	processor.C\
	property.C\
	selectable.C\
	textPersistenceManager.C\
	timeStamp.C\
	visitor.C

ifeq ($(BALL_HAS_XDR),true)
	CPP_SOURCES += XDRPersistenceManager.C
endif

include ../rules.mak
include ../targets.mak

-include .Dependencies
