# -*- Mode: C++; tab-width: 2; -*-
# vi: set ts=2:
#
# $Id: Makefile,v 1.41 2003/08/27 16:40:08 amoll Exp $
#

EXECUTABLE=molview
DIRECTORY=APPLICATIONS/MOLVIEW

all:	$(EXECUTABLE)

clean:	default_clean

include ../../common.mak
include .Dependencies

# replace library-specific flags by application-
# specific compiler flags
LIB_CXXFLAGS=$(NONLIB_CXXFLAGS) -DQT_THREAD_SUPPORT

BALL_INCLUDES=$(VIEW_INCLUDES) $(PYTHON_INCLUDES)

CPP_SOURCES=\
	icons.C\
	main.C\
	mainframe.C\
 	threads.C

MOC_SOURCES=\
	mainframe_moc.C
	
#	aboutDialog_moc.C\

molview:	$(OBJECTS)	$(SPECIAL_OBJECTS)
	$(CXX)	$(LDFLAGS) -o $(EXECUTABLE) $(OBJECTS) $(SPECIAL_OBJECTS) $(VIEW_LIBS) $(BALL_INCLUDES) $(PYTHON_LIBS) -DQT_THREAD_SUPPORT

aboutDialog_moc.C: aboutDialog.h
				$(MOC) aboutDialog.h -o $@

mainframe_moc.C:	mainframe.h
					$(MOC) mainframe.h -o $@
