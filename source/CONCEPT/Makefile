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

ifeq ($(BALL_HAS_VIEW),true)
	ASIO_SOURCES=$(BALL_HAS_ASIO:true=client.C)
endif

CPP_SOURCES=$(ASIO_SOURCES)\
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
	molecularInformation.C

ifeq ($(BALL_HAS_XDR),true)
	CPP_SOURCES += XDRPersistenceManager.C
endif

ifeq ($(BALL_HAS_VIEW),true)
	CPP_SOURCES +=\
 	objectCreator.C\
	moleculeObjectCreator.C
endif

include ../rules.mak
include ../targets.mak

-include .Dependencies
