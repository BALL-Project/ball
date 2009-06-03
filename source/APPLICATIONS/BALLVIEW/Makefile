# -*- Mode: C++; tab-width: 2; -*-
# vi: set ts=2:
#
# $Id: Makefile,v 1.13.8.2 2007/03/26 07:11:32 amoll Exp $
#
# Author:
#   Oliver Kohlbacher
#   Andreas Moll
#

DIRECTORY=APPLICATIONS/BALLVIEW

EXECUTABLE=BALLView

all:	$(EXECUTABLE)

clean:	default_clean

include ../../common.mak
-include .Dependencies

UIC_DIR=.

CPP_SOURCES=\
	demoTutorialDialog.C\
	icons.C\
	main.C\
	mainframe.C

MOC_SOURCES=\
	demoTutorialDialog_moc.C\
	mainframe_moc.C

RCC_FILES=\
	splashscreen.rcc

UIC_FILES=\
	aboutDialog.h\
	demoTutorialDialogData.h

BALLView:	$(OBJECTS)	$(SPECIAL_OBJECTS)
	$(LINKER)	$(LDFLAGS) -L$(PREFIX)/$(LIBDIR) -L$(BALL_PATH)/$(LIBDIR) -o $(EXECUTABLE) $(OBJECTS) $(SPECIAL_OBJECTS) $(BALL_LIBS) $(PYTHON_LIBS)

depend: $(UIC_FILES) $(MOC_SOURCES) $(RCC_FILES)

mainframe_moc.C:	mainframe.h
	$(MOC) mainframe.h -o $@

aboutDialog.h: aboutDialog.ui
	$(UIC) aboutDialog.ui > aboutDialog.h
				
demoTutorialDialogData.h: demoTutorialDialog.ui
	$(UIC) demoTutorialDialog.ui > demoTutorialDialogData.h
				
demoTutorialDialogData.C: demoTutorialDialogData.h
	$(UIC) -impl demoTutorialDialogData.h demoTutorialDialog.ui > demoTutorialDialogData.C

demoTutorialDialog_moc.C: demoTutorialDialog.h
	$(MOC) demoTutorialDialog.h -o $@

ballviewDemoData_moc.C: demoTutorialDialogData.h
	$(MOC) demoTutorialDialogData.h -o $@

%.rcc: %.qrc
	${RCC} $< -o $@

clean: 
	@$(RM) -f $(UIC_FILES)
	@$(RM) -f *.o *moc*

macos_app: $(EXECUTABLE) BALLView.app/Contents/MacOS/../PkgInfo BALLView.app/Contents/MacOS/../Info.plist 
	@$(CP) BALLView BALLView.app/Contents/MacOS
	@test -d BALLView.app/Contents/Resources/ || mkdir -p BALLView.app/Contents/Resources/
	@$(CP) $(BALL_PATH)/MacOSX/BALLView.icns BALLView.app/Contents/Resources

BALLView.app/Contents/MacOS/../PkgInfo: 
	@test -d BALLView.app/Contents/MacOS/ || mkdir -p BALLView.app/Contents/MacOS/
	@-$(RM) -f BALLView.app/Contents/MacOS/../PkgInfo 2>/dev/null
	@echo "APPLBALL" >BALLView.app/Contents/MacOS/../PkgInfo

BALLView.app/Contents/MacOS/../Info.plist: 
	@test -d BALLView.app/Contents/MacOS/ || mkdir -p BALLView.app/Contents/MacOS/
	@-$(RM) -f BALLView.app/Contents/MacOS/../Info.plist 2>/dev/null
	@$(CP) $(BALL_PATH)/MacOSX/Info.plist.app BALLView.app/Contents/Info.plist

macos_pkg: macos_app
	@$(CP) -r $(BALL_PATH)/data BALLView.app/Contents/
	@macdeployqt BALLView.app	
	@$(CP) $(BALL_PATH)/lib/*.so BALLView.app/Contents/Frameworks
	@$(BALL_PATH)/MacOSX/fixnames.sh BALLView.app/Contents/Frameworks/BALLmodule.so $(BALL_PATH)/lib
	@$(BALL_PATH)/MacOSX/fixnames.sh BALLView.app/Contents/Frameworks/BALLmodule.so $(BALL_PATH)/contrib/lib
	@$(RM) -rf $(BALL_PATH)/MacOSX/BALLPackage/Applications/BALLView.app
	@$(CP) $(BALL_PATH)/MacOSX/Info.plist.app BALLView.app/Contents/Info.plist
	@$(MV) BALLView.app $(BALL_PATH)/MacOSX/BALLPackage/Applications
	@open -a PackageMaker $(BALL_PATH)/MacOSX/BALLView-package.pmdoc
	
