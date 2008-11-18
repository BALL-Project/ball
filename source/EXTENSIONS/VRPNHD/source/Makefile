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

LIBRARY=pluginVRPNHD.so

DIRECTORY=EXTENSIONS/VRPNHD/source

CPP_SOURCES=\
	VRPNHDPlugin.C\
	VRPNHDDriver.C\
	logo.C

MOC_SOURCES=\
	VRPNHDPlugin_moc.C

all: $(LIBRARY)

include ../../../common.mak
-include .Dependencies

#BALL_INCLUDES+=-I../include ${VRPN_INCLUDES}
BALL_INCLUDES+=-I../include ${VRPN_INCLUDES}
BALL_INCLUDES+=-I../include ${QUAT_INCLUDES}

clean: default_clean

%.so: $(OBJECTS)
	$(CXX) -shared $(LDFLAGS) -o $@ $(OBJECTS) ${VRPN_LIBS} ${QUAT_LIBS}

VRPNHDPlugin_moc.C: ../include/VRPNHDPlugin.h
	${MOC} ${BALL_INCLUDES} $< -o $@

logo.C: logo.qrc
	${RCC} $< -o $@

