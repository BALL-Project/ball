# -*- Mode: C++; tab-width: 2; -*-
# vi: set ts=2:
#
# $Id: Makefile,v 1.38 2003/08/26 18:35:35 amoll Exp $
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
//	DIALOGS/DlgAbout.C\
//	DIALOGS/DlgAboutData.C\
	icons.C\
	main.C\
	mainframe.C\
	threads.C

MOC_SOURCES=\
	DIALOGS/DlgAbout_moc.C\
	DIALOGS/DlgAboutData_moc.C\
	mainframe_moc.C

molview:	$(OBJECTS)	$(SPECIAL_OBJECTS)
	$(CXX)	$(LDFLAGS) -o $(EXECUTABLE) $(OBJECTS) $(SPECIAL_OBJECTS) $(BALL_INCLUDES) $(PYTHON_LIBS) -DQT_THREAD_SUPPORT

DIALOGS/DlgAbout_moc.C:     DIALOGS/DlgAbout.h
				$(MOC) DIALOGS/DlgAbout.h -o $@

DIALOGS/DlgAboutData_moc.C: DIALOGS/DlgAboutData.h
				$(MOC) DIALOGS/DlgAboutData.h -o $@

mainframe_moc.C:	mainframe.h
					$(MOC) mainframe.h -o $@
