# $Id: Makefile,v 1.3 2003/08/26 09:18:51 oliver Exp $
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

