# -*- Mode: C++; tab-width: 2; -*-
# vi: set ts=2:
#
# $Id: Makefile,v 1.1.2.8.2.2 2005/02/17 12:51:24 haid Exp $
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
	dockDialog.C\
	geometricFitDialog.C\
	dockingProgressDialog.C\
	dockDialogData.C\
	geometricFitDialogData.C\
	dockingProgressDialogData.C\
	icons.C\
	main.C\
	mainframe.C

MOC_SOURCES=\
	aboutDialog_moc.C\
	dockDialog_moc.C\
	dockDialogData_moc.C\
	geometricFitDialog_moc.C\
	geometricFitDialogData_moc.C\
	dockingProgressDialog_moc.C\
	dockingProgressDialogData_moc.C\
	mainframe_moc.C
	
UIC_FILES=\
	aboutDialog.C\
	aboutDialog.h\
	dockDialogData.C\
	dockDialogData.h\
	geometricFitDialogData.C\
	geometricFitDialogData.h\
	dockingProgressDialogData.C\
	dockingProgressDialogData.h

DOCKView:	$(OBJECTS)	$(SPECIAL_OBJECTS)
	$(CXX)	$(LDFLAGS) -o $(EXECUTABLE) $(OBJECTS) $(SPECIAL_OBJECTS) $(VIEW_LIBS) $(BALL_INCLUDES) $(PYTHON_LIBS) 

depend: $(UIC_FILES) $(MOC_SOURCES)

#header files
aboutDialog.h: aboutDialog.ui
	$(UIC) aboutDialog.ui > aboutDialog.h

dockDialogData.h: dockDialog.ui
	$(UIC) dockDialog.ui > dockDialogData.h

geometricFitDialogData.h: geometricFitDialog.ui
	$(UIC) geometricFitDialog.ui > geometricFitDialogData.h
	
dockingProgressDialogData.h: dockingProgressDialog.ui
	$(UIC) dockingProgressDialog.ui > dockingProgressDialogData.h
	
#moc C files
mainframe_moc.C:	mainframe.h
	$(MOC) mainframe.h -o $@

aboutDialog_moc.C: aboutDialog.h
	$(MOC) aboutDialog.h -o $@

dockDialog_moc.C: dockDialog.h
	$(MOC) dockDialog.h -o $@

dockDialogData_moc.C: dockDialogData.h
	$(MOC) dockDialogData.h -o $@

geometricFitDialog_moc.C: geometricFitDialog.h
	$(MOC) geometricFitDialog.h -o $@	
		
geometricFitDialogData_moc.C: geometricFitDialogData.h
	$(MOC) geometricFitDialogData.h -o $@
	
dockingProgressDialog_moc.C: dockingProgressDialog.h
	$(MOC) dockingProgressDialog.h -o $@
	
dockingProgressDialogData_moc.C: dockingProgressDialogData.h
	$(MOC) dockingProgressDialogData.h -o $@

#C files
aboutDialog.C: aboutDialog.h
	$(UIC) -impl aboutDialog.h aboutDialog.ui > aboutDialog.C

dockDialogData.C: dockDialogData.h
	$(UIC) -impl dockDialogData.h dockDialog.ui > dockDialogData.C

geometricFitDialogData.C: geometricFitDialogData.h
	$(UIC) -impl geometricFitDialogData.h geometricFitDialog.ui > geometricFitDialogData.C
	
dockingProgressDialogData.C: dockingProgressDialogData.h
	$(UIC) -impl dockingProgressDialogData.h dockingProgressDialog.ui > dockingProgressDialogData.C

clean: clean_moc_uic

clean_moc_uic:
	@$(RM) -f $(UIC_FILES)
