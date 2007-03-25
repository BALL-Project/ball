# -*- Mode: C++; tab-width: 2; -*-
# vi: set ts=2:
#
# $Id: Makefile,v 1.22.14.1 2007/03/25 22:00:04 oliver Exp $
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
	visitor.C\
	objectCreator.C\
	moleculeObjectCreator.C\
	molecularInformation.C\
	client.C

ifeq ($(BALL_HAS_XDR),true)
	CPP_SOURCES += XDRPersistenceManager.C
endif

include ../rules.mak
include ../targets.mak

-include .Dependencies
