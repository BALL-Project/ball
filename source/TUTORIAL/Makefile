# $Id: Makefile,v 1.5 2000/04/28 12:33:51 oliver Exp $
#========================
# BALL/TUTORIAL Makefile
#========================

EXECUTABLES=tutorial1 tutorial2

all:	$(EXECUTABLES)

include ../common.mak

# remove the library specific flags
BALL_CXXFLAGS=

clean:	default_clean
	@$(RM) $(EXECUTABLES)

tutorial1:	tutorial1.o
	$(CXX)	$(LDFLAGS) -o $@ $@.o $(LIBS)

tutorial2:	tutorial2.o
	$(CXX)	$(LDFLAGS) -o $@ $@.o $(LIBS)

