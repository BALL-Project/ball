# -*- Mode: C++; tab-width: 2; -*-
# vi: set ts=2:
#
# $Id: Makefile,v 1.221.14.3 2007/05/14 18:32:37 oliver Exp $
#
# Author:
#   Oliver Kohlbacher
#

include ../config.mak

DIRECTORY=BENCHMARKS

include Makefile.lst


# default target
all:  $(EXECUTABLES)

include ../rules.mak
include ../targets.mak
-include .Dependencies

# required for clean and depend
CPP_SOURCES=$(EXECUTABLES:$(EXEEXT)=.C)
OBJECTS=$(EXECUTABLES:$(EXEEXT)=.o)
VALGRINDS=$(EXECUTABLES:$(EXEEXT)=.vgr)

all:	$(EXECUTABLES)

# remove the library specific flags
LIB_CXXFLAGS=$(NONLIB_CXXFLAGS)

.o:
	$(CXX) $(CXXFLAGS) $*.o $(LDFLAGS) -o $@ $(LIBS)
# 
%%.C:
	$(CXX) $(CXXFLAGS) $(ADD_CXXFLAGS) $(CPP_MODE_FLAGS) $(BALL_CXXFLAGS) $(BALL_INCLUDES) $*.C $(LDFLAGS) -o $@ $(LIBS)

bench:	$(EXECUTABLES)
	@./runbenchmarks $(EXECUTABLES)

clean:	default_clean
	@$(RM) -rf *.o *~ *.log ii_files Templates.DB	SunWS_cache _*.TMP core a.out $(EXECUTABLES)
 
