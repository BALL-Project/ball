# $Id: Makefile,v 1.6 2002/01/12 21:22:44 anker Exp $
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

bounding_box:	bounding_box.o glBoundingBoxModel.o
	$(CXX)	$(LDFLAGS) -o $@ $@.o glBoundingBoxModel.o $(LIBS) $(BALLVIEW_LIBS)

