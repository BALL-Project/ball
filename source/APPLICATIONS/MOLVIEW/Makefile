# $Id: Makefile,v 1.36 2003/07/21 07:54:00 amoll Exp $

EXECUTABLE=molview
DIRECTORY=APPLICATIONS/MOLVIEW

all:	$(EXECUTABLE)

clean:	default_clean

include ../../common.mak
include .Dependencies

# replace library-specific flags by application-
# specific compiler flags
LIB_CXXFLAGS=$(NONLIB_CXXFLAGS) -DQT_THREAD_SUPPORT

BALL_INCLUDES=$(BALLVIEW_INCLUDES) $(PYTHON_INCLUDES)

CPP_SOURCES=\
	DIALOGS/DlgAbout.C\
	DIALOGS/DlgAboutData.C\
	icons.C\
	main.C\
	mainframe.C\
	threads.C

MOC_SOURCES=\
	DIALOGS/DlgAbout_moc.C\
	DIALOGS/DlgAboutData_moc.C\
	mainframe_moc.C


SPECIAL_OBJECTS=

molview:	$(OBJECTS)	$(SPECIAL_OBJECTS)
	$(CXX)	$(LDFLAGS) -o $(EXECUTABLE) $(OBJECTS) $(SPECIAL_OBJECTS) $(BALLVIEW_LIBS) $(PYTHON_LIBS) -DQT_THREAD_SUPPORT

example: example02.o 
	$(CXX)	$(LDFLAGS) -o example example02.o $(BALLVIEW_LIBS) $(PYTHON_LIBS)

DIALOGS/DlgAbout_moc.C:     DIALOGS/DlgAbout.h
				$(MOC) DIALOGS/DlgAbout.h -o $@

DIALOGS/DlgAboutData_moc.C: DIALOGS/DlgAboutData.h
				$(MOC) DIALOGS/DlgAboutData.h -o $@

mainframe_moc.C:	mainframe.h
					$(MOC) mainframe.h -o $@
