# $Id: Makefile,v 1.2 2000/03/24 11:13:34 oliver Exp $
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
