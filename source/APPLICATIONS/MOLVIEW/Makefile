# -*- Mode: C++; tab-width: 2; -*-
# vi: set ts=2:
#
# $Id: Makefile,v 1.44 2003/08/29 10:57:16 amoll Exp $
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
	aboutDialog.C\
	icons.C\
	main.C\
	mainframe.C\
 	threads.C

MOC_SOURCES=\
	aboutDialog_moc.C\
	mainframe_moc.C
	
UIC_SOURCES=\
	aboutDialogData.C


molview:	$(OBJECTS)	$(SPECIAL_OBJECTS)
	$(CXX)	$(LDFLAGS) -o $(EXECUTABLE) $(OBJECTS) $(SPECIAL_OBJECTS) $(VIEW_LIBS) $(BALL_INCLUDES) $(PYTHON_LIBS) -DQT_THREAD_SUPPORT

aboutDialog.h: aboutDialog.ui
	$(UIC) aboutDialog.ui > aboutDialog.h
				
aboutDialog_moc.C: aboutDialog.h
	$(MOC) aboutDialog.h -o $@

mainframe_moc.C:	mainframe.h
	$(MOC) mainframe.h -o $@

$(UIC_SOURCES): $(UIC_HDR)

depend: $(UIC_SOURCES) $(MOC_SOURCES)
#
#aboutDialog.C: aboutDialog.h
#	$(UIC) -impl aboutDialog.h aboutDialog.ui > aboutDialog.C
#				
#
