# $Id: Makefile,v 1.2 2003/06/09 21:19:31 oliver Exp $
#========================
# BALL/EXAMPLES Makefile
#========================

DIRECTORY=config
EXECUTABLES=check_test

all:	$(EXECUTABLES)

include ../common.mak

# remove the library specific flags
LIB_CXXFLAGS = $(NONLIB_CXXFLAGS)

clean:	default_clean clean_flex_bison
	@-$(RM) $(EXECUTABLES) *.o 2>/dev/null

check_test:	check_test.o 
	$(CXX)	$(LDFLAGS) -o $@ $@.o

clean_flex_bison:
	@-$(RM) check_test.C

check_test.o: check_test.C
	$(CXX) $(CXXFLAGS) $(ADD_CXXFLAGS) $(CPP_MODE_FLAGS) -c $*.C -o $@

