# -*- Mode: C++; tab-width: 2; -*-
# vi: set ts=2:
#
# $Id: Makefile,v 1.1.2.3 2005/01/19 09:41:46 leonhardt Exp $
#

EXECUTABLE=DOCKView
DIRECTORY=APPLICATIONS/DOCKVIEW

all:	$(EXECUTABLE)

clean:	default_clean

include ../../common.mak
include .Dependencies

BALL_INCLUDES=$(VIEW_INCLUDES) $(PYTHON_INCLUDES)

CPP_SOURCES=\
	aboutDialog.C\
	#dockAlgorithm.C\
	dockDialog.C\
	dockDialogData.C\
	icons.C\
	main.C\
	mainframe.C

MOC_SOURCES=\
	aboutDialog_moc.C\
	dockDialog_moc.C\
	dockDialogData_moc.C\
	mainframe_moc.C
	
UIC_FILES=\
	aboutDialog.C\
	aboutDialog.h\
	dockDialogData.C\
	dockDialogData.h

DOCKView:	$(OBJECTS)	$(SPECIAL_OBJECTS)
	$(CXX)	$(LDFLAGS) -o $(EXECUTABLE) $(OBJECTS) $(SPECIAL_OBJECTS) $(VIEW_LIBS) $(BALL_INCLUDES) $(PYTHON_LIBS)

depend: $(UIC_FILES) $(MOC_SOURCES)

#header files
aboutDialog.h: aboutDialog.ui
	$(UIC) aboutDialog.ui > aboutDialog.h

dockDialogData.h: dockDialogData.ui
	$(UIC) dockDialog.ui > dockDialogData.h

#moc C files
mainframe_moc.C:	mainframe.h
	$(MOC) mainframe.h -o $@

aboutDialog_moc.C: aboutDialog.h
	$(MOC) aboutDialog.h -o $@

dockDialog_moc.C: dockDialog.h
	$(MOC) dockDialog.h -o $@

dockDialogData_moc.C: dockDialogData.h
	$(MOC) dockDialogData.h -o $@

#C files
aboutDialog.C: aboutDialog.h
	$(UIC) -impl aboutDialog.h aboutDialog.ui > aboutDialog.C

dockDialogData.C: dockDialogData.h
	$(UIC) -impl dockDialogData.h dockDialog.ui > dockDialogData.C


clean: clean_moc_uic

clean_moc_uic:
	@$(RM) -f $(UIC_FILES)
