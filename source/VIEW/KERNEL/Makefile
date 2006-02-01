# -*- Mode: C++; tab-width: 2; -*-
# vi: set ts=2:
#
# $Id: Makefile,v 1.22.2.3 2006/02/01 14:15:05 amoll Exp $
#
# Author:
#   Oliver Kohlbacher
#

DIRECTORY=VIEW/KERNEL

THISLIB=$(LIBNAME)

CPP_SOURCES=\
  client.C\
  clippingPlane.C\
	common.C\
  compositeManager.C\
  connectionObject.C\
  geometricObject.C\
  mainControl.C\
  message.C\
  modularWidget.C\
  molecularInformation.C\
  moleculeObjectCreator.C\
  objectCreator.C\
	preferencesEntry.C\
  representationManager.C\
  representation.C\
  server.C\
  stage.C\
	threads.C

MOC_SOURCES=\
	mainControl_moc.C

include ../../common.mak
-include .Dependencies

mainControl_moc.C:  $(BALL_PATH)/include/BALL/VIEW/KERNEL/mainControl.h
	$(MOC) $(BALL_PATH)/include/BALL/VIEW/KERNEL/mainControl.h -o mainControl_moc.C
 
