# -*- Mode: C++; tab-width: 2; -*-
# vi: set ts=2:
#
# $Id: Makefile,v 1.1.2.8.2.12 2005/07/18 13:40:12 leonhardt Exp $
#

EXECUTABLE=DOCKView
DIRECTORY=APPLICATIONS/DOCKVIEW

all:	$(EXECUTABLE)

clean:	default_clean

include ../../common.mak
-include .Dependencies

BALL_INCLUDES=$(VIEW_INCLUDES) $(PYTHON_INCLUDES) 

CPP_SOURCES=\
	aboutDialog.C\
	dockDialog.C\
	dockingController.C\
	geometricFitDialog.C\
	dockProgressDialog.C\
	dockResultDialog.C\
	dockDialogData.C\
	geometricFitDialogData.C\
	dockProgressDialogData.C\
	dockResultDialogData.C\
	dockResult.C\
	icons.C\
	main.C\
	mainframe.C

MOC_SOURCES=\
	aboutDialog_moc.C\
	dockDialog_moc.C\
	dockDialogData_moc.C\
	dockingController_moc.C\
	geometricFitDialog_moc.C\
	geometricFitDialogData_moc.C\
	dockProgressDialog_moc.C\
	dockProgressDialogData_moc.C\
	dockResultDialog_moc.C\
	dockResultDialogData_moc.C\
	mainframe_moc.C
	
UIC_FILES=\
	aboutDialog.C\
	aboutDialog.h\
	dockDialogData.C\
	dockDialogData.h\
	geometricFitDialogData.C\
	geometricFitDialogData.h\
	dockProgressDialogData.C\
	dockProgressDialogData.h\
	dockResultDialogData.h\
	dockResultDialogData.C

DOCKView:	$(OBJECTS)	$(SPECIAL_OBJECTS)
	$(CXX)	$(LDFLAGS) -o $(EXECUTABLE) $(OBJECTS) $(SPECIAL_OBJECTS) $(VIEW_LIBS) $(BALL_INCLUDES) $(PYTHON_LIBS) $(FFTW_LIBS)

depend: $(UIC_FILES) $(MOC_SOURCES)

#header files
aboutDialog.h: aboutDialog.ui
	$(UIC) aboutDialog.ui > aboutDialog.h

dockDialogData.h: dockDialog.ui
	$(UIC) dockDialog.ui > dockDialogData.h
	
geometricFitDialogData.h: geometricFitDialog.ui
	$(UIC) geometricFitDialog.ui > geometricFitDialogData.h
	
dockProgressDialogData.h: dockProgressDialog.ui
	$(UIC) dockProgressDialog.ui > dockProgressDialogData.h
	
dockResultDialogData.h: dockResultDialog.ui
	$(UIC) dockResultDialog.ui > dockResultDialogData.h	
	
#moc C files
mainframe_moc.C:	mainframe.h
	$(MOC) mainframe.h -o $@

aboutDialog_moc.C: aboutDialog.h
	$(MOC) aboutDialog.h -o $@
	
dockingController_moc.C: dockingController.h
	$(MOC) dockingController.h -o $@

dockDialog_moc.C: dockDialog.h
	$(MOC) dockDialog.h -o $@

dockDialogData_moc.C: dockDialogData.h
	$(MOC) dockDialogData.h -o $@
	
geometricFitDialog_moc.C: geometricFitDialog.h
	$(MOC) geometricFitDialog.h -o $@	
		
geometricFitDialogData_moc.C: geometricFitDialogData.h
	$(MOC) geometricFitDialogData.h -o $@
	
dockProgressDialog_moc.C: dockProgressDialog.h
	$(MOC) dockProgressDialog.h -o $@
	
dockProgressDialogData_moc.C: dockProgressDialogData.h
	$(MOC) dockProgressDialogData.h -o $@
	
dockResultDialog_moc.C: dockResultDialog.h
	$(MOC) dockResultDialog.h -o $@

dockResultDialogData_moc.C: dockResultDialogData.h
	$(MOC) dockResultDialogData.h -o $@

#C files
aboutDialog.C: aboutDialog.h
	$(UIC) -impl aboutDialog.h aboutDialog.ui > aboutDialog.C

dockDialogData.C: dockDialogData.h
	$(UIC) -impl dockDialogData.h dockDialog.ui > dockDialogData.C

geometricFitDialogData.C: geometricFitDialogData.h
	$(UIC) -impl geometricFitDialogData.h geometricFitDialog.ui > geometricFitDialogData.C
	
dockProgressDialogData.C: dockProgressDialogData.h
	$(UIC) -impl dockProgressDialogData.h dockProgressDialog.ui > dockProgressDialogData.C
	
dockResultDialogData.C: dockResultDialogData.h
	$(UIC) -impl dockResultDialogData.h dockResultDialog.ui > dockResultDialogData.C

clean: clean_moc_uic

clean_moc_uic:
	@$(RM) -f $(UIC_FILES)
