# -*- Mode: C++; tab-width: 2; -*-
# vi: set ts=2:
#
# $Id: Makefile,v 1.5 2006/01/26 16:12:53 oliver Exp $
#
# Author:
#   Andreas Moll
#   Oliver Kohlbacher
#

DIRECTORY=APPLICATIONS/DCD2PNG

EXECUTABLE=DCD2PNG

all:	$(EXECUTABLE)

clean:	default_clean

include ../../common.mak
-include .Dependencies

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
