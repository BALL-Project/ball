# $Id: Makefile,v 1.33.4.1 2002/10/16 00:45:43 amoll Exp $

EXECUTABLE=molview
DIRECTORY=APPLICATIONS/MOLVIEW

all:	$(EXECUTABLE)

clean:	default_clean

include ../../common.mak
include .Dependencies

# replace library-specific flags by application-
# specific compiler flags
LIB_CXXFLAGS=$(NONLIB_CXXFLAGS)

BALL_INCLUDES+=$(BALLVIEW_INCLUDES) $(PYTHON_INCLUDES)

CPP_SOURCES=\
	DIALOGS/DlgAbout.C\
	DIALOGS/DlgAboutData.C\
	DIALOGS/DlgAmberMinimization.C\
	DIALOGS/DlgAmberMinimizationData.C\
	icons.C\
	main.C\
	mainframe.C\

MOC_SOURCES=\
	DIALOGS/DlgAbout_moc.C\
	DIALOGS/DlgAboutData_moc.C\
	DIALOGS/DlgAmberMinimization_moc.C\
	DIALOGS/DlgAmberMinimizationData_moc.C\
	mainframe_moc.C


SPECIAL_OBJECTS=

molview:	$(OBJECTS) $(SPECIAL_OBJECTS)
	$(CXX)	$(LDFLAGS) -o $(EXECUTABLE) $(OBJECTS) $(SPECIAL_OBJECTS) $(BALLVIEW_LIBS) $(PYTHON_LIBS)

DIALOGS/DlgAbout_moc.C:     DIALOGS/DlgAbout.h
				$(MOC) DIALOGS/DlgAbout.h -o $@

DIALOGS/DlgAboutData_moc.C: DIALOGS/DlgAboutData.h
				$(MOC) DIALOGS/DlgAboutData.h -o $@

DIALOGS/DlgAmberMinimization_moc.C:     DIALOGS/DlgAmberMinimization.h
				$(MOC) DIALOGS/DlgAmberMinimization.h -o $@

DIALOGS/DlgAmberMinimizationData_moc.C: DIALOGS/DlgAmberMinimizationData.h
				$(MOC) DIALOGS/DlgAmberMinimizationData.h -o $@

mainframe_moc.C:	mainframe.h
					$(MOC) mainframe.h -o $@
