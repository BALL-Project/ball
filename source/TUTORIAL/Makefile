# $Id: Makefile,v 1.11 2004/02/19 21:27:06 amoll Exp $
#========================
# BALL/TUTORIAL Makefile
#========================

EXECUTABLES=\
	tutorial1\
	tutorial2\
	tutorial3\
	bounding_box

all:	$(EXECUTABLES)

include ../common.mak

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

bounding_box:	bounding_box.o glBoundingBoxModel.o
	$(CXX)	$(LDFLAGS) -o $@ $@.o glBoundingBoxModel.o $(LIBS) $(VIEW_LIBS)

string_tutorial:	string_tutorial.o 
	$(CXX)	$(LDFLAGS) -o $@ $@.o $(LIBS) 

