# $Id: Makefile,v 1.4 2000/03/27 19:58:56 oliver Exp $
#========================
# BALL/TUTORIAL Makefile
#========================

EXECUTABLES=tutorial1

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

