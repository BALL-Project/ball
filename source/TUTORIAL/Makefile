# -*- Mode: C++; tab-width: 2; -*-
# vi: set ts=2:
#
# $Id: Makefile,v 1.14.18.6 2007/08/07 19:59:23 oliver Exp $
#
# Author:
#   Oliver Kohlbacher
#

DIRECTORY=TUTORIAL

include ../common.mak

default: all

EXECUTABLES=\
	tutorial1\
	tutorial2\
	tutorial3\
	string_tutorial

VIEW_DEPENDENT_EXECUTABLES=\
	bounding_box

ifeq ($(BALL_HAS_VIEW),true)
  EXECUTABLES_OTHERS += $(VIEW_DEPENDENT_EXECUTABLES)
endif
		
# default target
all:	$(EXECUTABLES)

# required for clean and depend
CPP_SOURCES=$(EXECUTABLES:=.C)
OBJECTS=$(EXECUTABLES:=.o)


-include .Dependencies

# remove the library specific flags
# and replace them by the application specific
# flags
LIB_CXXFLAGS=$(NONLIB_CXXFLAGS)

.o:
	$(CXX) $(CXXFLAGS) $*.o $(LDFLAGS) -o $@ $(LIBS)

%%.C:
	$(CXX) $(CXXFLAGS) $(ADD_CXXFLAGS) $(CPP_MODE_FLAGS) $(BALL_CXXFLAGS) $(BALL_INCLUDES) $*.C $(LDFLAGS) -o $@ $(LIBS)

clean:  default_clean
	@$(RM) -rf *.o *~ *.log ii_files Templates.DB   SunWS_cache _*.TMP core a.out $(EXECUTABLES)
