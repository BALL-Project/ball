# -*- Mode: C++; tab-width: 2; -*-
# vi: set ts=2:
#
# $Id: Makefile,v 1.1 2004/07/15 15:20:14 amoll Exp $
#

EXECUTABLE=DCD2PNG
DIRECTORY=APPLICATIONS/DCD2PNG

all:	$(EXECUTABLE)

clean:	default_clean

include ../../common.mak
include .Dependencies

BALL_INCLUDES=$(VIEW_INCLUDES) $(PYTHON_INCLUDES)

CPP_SOURCES=\
	main.C\
	mainframe.C

MOC_SOURCES=\
	mainframe_moc.C
	
DCD2PNG:	$(OBJECTS)	$(SPECIAL_OBJECTS)
	$(CXX)	$(LDFLAGS) -o $(EXECUTABLE) $(OBJECTS) $(SPECIAL_OBJECTS) $(VIEW_LIBS) $(BALL_INCLUDES)

depend: $(MOC_SOURCES)

mainframe_moc.C:	mainframe.h
	$(MOC) mainframe.h -o $@

clean: clean_moc_uic

clean_moc_uic:
	@$(RM) -f $(UIC_FILES)
