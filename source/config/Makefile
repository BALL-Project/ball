# $Id: Makefile,v 1.1 2003/06/09 14:17:13 oliver Exp $
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
	$(CXX)	$(LDFLAGS) -o $@ $@.o $(LIBS)

clean_flex_bison:
	@-$(RM) expressionParserParser.C expressionParserLexer.C expressionParserParser.h

check_Test.o: check_test.C
	$(CXX) $(CXXFLAGS) $(ADD_CXXFLAGS) $(CPP_MODE_FLAGS) $(LIB_CXXFLAGS) $(BALL_INCLUDES) $(ADD_INCLUDES) -c $*.C -o $@

