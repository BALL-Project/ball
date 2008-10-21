# -*- Mode: C++; tab-width: 2; -*-
# vi: set ts=2:
#
# $Id: Makefile,v 1.13.8.2 2007/03/26 07:11:32 amoll Exp $
#
# Author:
#  Daniel Stoeckel
#

.PHONY: all clean depend

LIBRARY=pluginSpaceNavigator.so

DIRECTORY=PLUGINS/SPACENAV/source

CPP_SOURCES=\
	spaceNavigatorPlugin.C\
	spaceNavigatorDriver.C\
	logo.C

MOC_SOURCES=\
	spaceNavigatorPlugin_moc.C

all: $(LIBRARY)

include ../../../common.mak
-include .Dependencies

LIBS=-lspnav

BALL_INCLUDES+=-I../include

clean: default_clean

%.so: $(OBJECTS)
	$(CXX) -shared $(LDFLAGS) -o $@ $(OBJECTS) -L$(SPACENAV_LIBDIR) $(LIBS)

spaceNavigatorPlugin_moc.C: ../include/spaceNavigatorPlugin.h
	${MOC} ${BALL_INCLUDES} $< -o $@

logo.C: logo.qrc
	rcc $< -o $@

