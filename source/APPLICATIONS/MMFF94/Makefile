#
# $Id: Makefile,v 1.1.2.1 2005/03/22 00:32:39 amoll Exp $
# Makefile for BALL utilities
#

DIRECTORY=APPLICATIONS/MMFF94

EXECUTABLES = \
	splitMMFFTestSuiteMolFiles\
	splitMMFFTestSuiteOptiFiles\
	readMMFF94TestFile

# default target
all:	$(EXECUTABLES)

# required for clean and depend
CPP_SOURCES=$(EXECUTABLES:=.C)
OBJECTS=$(EXECUTABLES:=.o)

include ../../common.mak
include .Dependencies

# remove the library specific flags
# and replace them by the application specific
# flags
LIB_CXXFLAGS=$(NONLIB_CXXFLAGS)

.o:
	$(CXX) $(CXXFLAGS) $*.o $(LDFLAGS) -o $@ $(LIBS)

%%.C:
	$(CXX) $(CXXFLAGS) $(ADD_CXXFLAGS) $(CPP_MODE_FLAGS) $(BALL_CXXFLAGS) $(BALL_INCLUDES) $*.C $(LDFLAGS) -o $@ $(LIBS)

clean:  default_clean
	@$(RM) -rf *.o *~ *.log _*.TMP core a.out $(EXECUTABLES)
