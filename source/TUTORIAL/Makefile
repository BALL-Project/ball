# $Id: Makefile,v 1.11.6.3 2005/11/19 19:02:07 amoll Exp $
#========================
# BALL/TUTORIAL Makefile
#========================

EXECUTABLES=\
	tutorial1\
	tutorial2\
	tutorial3\
	bounding_box\
	string_tutorial

all:	$(EXECUTABLES)

include ../common.mak
-include .Dependencies

# remove the library specific flags
BALL_CXXFLAGS=

BALL_INCLUDES=$(VIEW_INCLUDES)

clean:	default_clean
	@-$(RM) *.o 2>/dev/null

tutorial1:	tutorial1.o
	$(CXX)	$(LDFLAGS) -o $@ $@.o $(LIBS)

tutorial2:	tutorial2.o
	$(CXX)	$(LDFLAGS) -o $@ $@.o $(LIBS)

tutorial3:	tutorial3.o
	$(CXX)	$(LDFLAGS) -o $@ $@.o $(LIBS)

bounding_box:	bounding_box.o 
	$(CXX)	$(LDFLAGS) -o $@ $(LIBS) $(VIEW_LIBS)

string_tutorial:	string_tutorial.o 
	$(CXX)	$(LDFLAGS) -o $@ $@.o $(LIBS) 

