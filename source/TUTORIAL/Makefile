# $Id: Makefile,v 1.3 2000/03/27 09:09:01 oliver Exp $
#========================
# BALL/TUTORIAL Makefile
#========================

EXECUTABLES=tutorial1

all:	$(EXECUTABLES)

include ../common.mak

clean:	default_clean
	@$(RM) $(EXECUTABLES)

tutorial1:	tutorial1.o
	$(CXX)	$(LDFLAGS) -o $@ $@.o $(LIBS)

tutorial2:	tutorial2.o
	$(CXX)	$(LDFLAGS) -o $@ $@.o $(LIBS)

