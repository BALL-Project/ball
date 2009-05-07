# -*- Mode: C++; tab-width: 2; -*-
# vi: set ts=2:
#
# Author:
#  Daniel Stoeckel
#

all: subdirs

include ../common.mak
-include .Dependencies

.PHONY: all clean depend subdirs

DIRECTORY=EXTENSIONS

THISLIB=$(LIBNAME)

ifeq ($(SPACENAV_SUPPORT), true)
SUBDIRS+="SPACENAV"
endif

SUBDIRS+=VRPN VRPNHD MULTITOUCH

depend: subdirs_depend

subdirs_depend:
	@$(MAKE) subdirs TARGET=depend

clean: default_clean
	@$(MAKE) subdirs TARGET=clean

subdirs:
	@for i in $(SUBDIRS); do echo "Entering $$i"; cd $$i; $(MAKE) $(TARGET); done;

