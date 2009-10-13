# -*- Mode: C++; tab-width: 2; -*-
# vi: set ts=2:
#
# $Id: Makefile,v 1.23.14.1 2007/03/25 22:02:21 oliver Exp $
#
# Author:
#   Oliver Kohlbacher
#

DIRECTORY=VIEW/KERNEL

THISLIB=$(LIBNAME)

ASIO_SOURCES=$(BALL_HAS_ASIO:true=serverWidget.C)
ASIO_MOC_SOURCES=$(BALL_HAS_ASIO:true=serverWidget_moc.C)

CPP_SOURCES=$(ASIO_SOURCES)\
  clippingPlane.C\
	common.C\
  compositeManager.C\
  connectionObject.C\
  geometricObject.C\
	iconLoader.C\
  mainControl.C\
  message.C\
  modelInformation.C\
  modularWidget.C\
	preferencesEntry.C\
  representationManager.C\
  representation.C\
	shortcutRegistry.C\
  stage.C\
	threads.C

MOC_SOURCES=$(ASIO_MOC_SOURCES)\
	mainControl_moc.C\
	shortcutRegistry_moc.C

include ../../common.mak
-include .Dependencies

mainControl_moc.C:  $(BALL_PATH)/include/BALL/VIEW/KERNEL/mainControl.h
	$(MOC) $(BALL_PATH)/include/BALL/VIEW/KERNEL/mainControl.h -o mainControl_moc.C
 
shortcutRegistry_moc.C:  $(BALL_PATH)/include/BALL/VIEW/KERNEL/shortcutRegistry.h
	$(MOC) $^ -o $@

serverWidget_moc.C:  $(BALL_PATH)/include/BALL/VIEW/KERNEL/serverWidget.h
	$(MOC) $(BALL_PATH)/include/BALL/VIEW/KERNEL/serverWidget.h -o serverWidget_moc.C
 

