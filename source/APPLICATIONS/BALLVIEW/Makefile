# -*- Mode: C++; tab-width: 2; -*-
# vi: set ts=2:
#
# $Id: Makefile,v 1.1 2004/04/16 11:24:47 amoll Exp $
#

EXECUTABLE=BALLView
DIRECTORY=APPLICATIONS/BALLVIEW

all:	$(EXECUTABLE)

clean:	default_clean

include ../../common.mak
include .Dependencies

BALL_INCLUDES=$(VIEW_INCLUDES) $(PYTHON_INCLUDES)

CPP_SOURCES=\
	aboutDialog.C\
	icons.C\
	main.C\
	mainframe.C

MOC_SOURCES=\
	aboutDialog_moc.C\
	mainframe_moc.C
	
UIC_FILES=\
	aboutDialog.C\
	aboutDialog.h

BALLView:	$(OBJECTS)	$(SPECIAL_OBJECTS)
	$(CXX)	$(LDFLAGS) -o $(EXECUTABLE) $(OBJECTS) $(SPECIAL_OBJECTS) $(VIEW_LIBS) $(BALL_INCLUDES) $(PYTHON_LIBS)

depend: $(UIC_FILES) $(MOC_SOURCES)

aboutDialog.h: aboutDialog.ui
	$(UIC) aboutDialog.ui > aboutDialog.h
				
mainframe_moc.C:	mainframe.h
	$(MOC) mainframe.h -o $@

aboutDialog_moc.C: aboutDialog.h
	$(MOC) aboutDialog.h -o $@

aboutDialog.C: aboutDialog.h
	$(UIC) -impl aboutDialog.h aboutDialog.ui > aboutDialog.C

clean: clean_moc_uic

clean_moc_uic:
	@$(RM) -f $(UIC_FILES)
