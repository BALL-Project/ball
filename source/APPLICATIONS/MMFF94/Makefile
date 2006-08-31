#
# $Id: Makefile,v 1.1.6.1 2006/08/31 14:05:08 leonhardt Exp $
# Makefile for BALL utilities
#

DIRECTORY=APPLICATIONS/MMFF94

EXECUTABLES = \
	splitMMFFTestSuiteMolFiles\
	splitMMFFTestSuiteOptiFiles\
	readMMFF94TestFile

# default target
all:	$(EXECUTABLES)

include ../../common.mak
-include .Dependencies

# required for clean and depend
CPP_SOURCES=$(EXECUTABLES:=.C)
OBJECTS=$(EXECUTABLES:=.o)

.o:
	$(CXX) $(CXXFLAGS) $*.o $(LDFLAGS) -o $@ $(LIBS)

%%.C:
	$(CXX) $(CXXFLAGS) $(ADD_CXXFLAGS) $(CPP_MODE_FLAGS) $(BALL_CXXFLAGS) $(BALL_INCLUDES) $*.C $(LDFLAGS) -o $@ $(LIBS)

clean:  default_clean
	@$(RM) -rf *.o *~ *.log _*.TMP core a.out $(EXECUTABLES)
