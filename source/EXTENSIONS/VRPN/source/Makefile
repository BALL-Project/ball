# -*- Mode: C++; tab-width: 2; -*-
# vi: set ts=2:
#
# $Id: Makefile,v 1.13.8.2 2007/03/26 07:11:32 amoll Exp $
#
# Author:
#  Stefan Nickels
#

.PHONY: all clean depend

VRPN_INCLUDES = -I/home/cyphr/HIWI/software/vrpn
QUAT_INCLUDES = -I/home/cyphr/HIWI/software/quat
VRPN_LIBS = -L/home/cyphr/HIWI/software/vrpn/pc_linux/ -lvrpn
QUAT_LIBS = -L/home/cyphr/HIWI/software/quat/pc_linux/ -lquat

LIBRARY=pluginVRPN.so

DIRECTORY=EXTENSIONS/VRPN/source

CPP_SOURCES=\
	VRPNPlugin.C\
	VRPNDriver.C\
	logo.C

MOC_SOURCES=\
	VRPNPlugin_moc.C

all: $(LIBRARY)

include ../../../common.mak
-include .Dependencies

#BALL_INCLUDES+=-I../include ${VRPN_INCLUDES}
BALL_INCLUDES+=-I../include ${VRPN_INCLUDES}
BALL_INCLUDES+=-I../include ${QUAT_INCLUDES}

clean: default_clean

%.so: $(OBJECTS)
	$(CXX) -shared $(LDFLAGS) -o $@ $(OBJECTS) ${VRPN_LIBS} ${QUAT_LIBS}

VRPNPlugin_moc.C: ../include/VRPNPlugin.h
	${MOC} ${BALL_INCLUDES} $< -o $@

logo.C: logo.qrc
	${RCC} $< -o $@

