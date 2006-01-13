# -*- Mode: C++; tab-width: 2; -*-
# vi: set ts=2:
#
# $Id: Makefile,v 1.10.2.2 2006/01/13 16:00:15 amoll Exp $
#

EXECUTABLE=BALLView
DIRECTORY=APPLICATIONS/BALLVIEW

all:	$(EXECUTABLE)

clean:	default_clean

include ../../common.mak
-include .Dependencies

UIC_DIR=.

BALL_INCLUDES=$(VIEW_INCLUDES) $(PYTHON_INCLUDES)

CPP_SOURCES=\
	demoTutorialDialog.C\
	icons.C\
	main.C\
	mainframe.C

MOC_SOURCES=\
	demoTutorialDialog_moc.C\
	mainframe_moc.C

UIC_FILES=\
	aboutDialog.h\
	demoTutorialDialogData.h

BALLView:	$(OBJECTS)	$(SPECIAL_OBJECTS)
	$(CXX)	$(LDFLAGS) -o $(EXECUTABLE) $(OBJECTS) $(SPECIAL_OBJECTS) $(VIEW_LIBS) $(PYTHON_LIBS)

depend: $(UIC_FILES) $(MOC_SOURCES)

mainframe_moc.C:	mainframe.h
	$(MOC) mainframe.h -o $@

demoTutorialDialog_moc.C:	demoTutorialDialog.h
	$(MOC) demoTutorialDialog.h -o $@


aboutDialog.h: aboutDialog.ui
	$(UIC) aboutDialog.ui > aboutDialog.h
				
demoTutorialDialogData.h: demoTutorialDialog.ui
	$(UIC) demoTutorialDialog.ui > demoTutorialDialogData.h
				
clean: 
	@$(RM) -f $(UIC_FILES)
	@$(RM) -f *.o *moc*

macos_app: $(EXECUTABLE) BALLView.app/Contents/MacOS/../PkgInfo BALLView.app/Contents/MacOS/../Info.plist 
	@$(CP) BALLView BALLView.app/Contents/MacOS

BALLView.app/Contents/MacOS/../PkgInfo: 
	@test -d BALLView.app/Contents/MacOS/ || mkdir -p BALLView.app/Contents/MacOS/
	@-$(RM) -f BALLView.app/Contents/MacOS/../PkgInfo 2>/dev/null
	@echo "APPL????" >BALLView.app/Contents/MacOS/../PkgInfo

BALLView.app/Contents/MacOS/../Info.plist: 
	@test -d BALLView.app/Contents/MacOS/ || mkdir -p BALLView.app/Contents/MacOS/
	@-$(RM) -f BALLView.app/Contents/MacOS/../Info.plist 2>/dev/null
	@sed -e "s,@ICON@,application.icns,g" -e "s,@EXECUTABLE@,BALLView,g" "../../../MacOSX/BALLView/Info.plist.app" >"BALLView.app/Contents/MacOS/../Info.plist"


macos_pkg: $(EXECUTABLE)
	@echo "Cleaning old package contents..."
	@-$(RM) -fr $(BALL_PATH)/MacOSX/BALLPackage/Library/BALL/Library/lib*dylib 2>/dev/null | true
	@-$(RM) -fr $(BALL_PATH)/MacOSX/BALLPackage/Applications/BALLView.app/Contents/MacOS/BALLView 2>/dev/null | true
	@-mkdir $(BALL_PATH)/MacOSX/BALLPackage 2>/dev/null | true
	@-mkdir $(BALL_PATH)/MacOSX/BALLPackage/Library 2>/dev/null | true
	@-mkdir $(BALL_PATH)/MacOSX/BALLPackage/Library/BALL 2>/dev/null | true
	@-mkdir $(BALL_PATH)/MacOSX/BALLPackage/Library/BALL/Library 2>/dev/null | true
	@-mkdir $(BALL_PATH)/MacOSX/BALLPackage/Applications 2>/dev/null | true
	@-mkdir $(BALL_PATH)/MacOSX/BALLPackage/Applications/BALLView.app 2>/dev/null | true
	@-mkdir $(BALL_PATH)/MacOSX/BALLPackage/Applications/BALLView.app/Contents 2>/dev/null | true
	@-mkdir $(BALL_PATH)/MacOSX/BALLPackage/Applications/BALLView.app/Contents/MacOS 2>/dev/null | true
	@echo "Copying BALL/data..."
	@$(CP) -r $(BALL_PATH)/data $(BALL_PATH)/MacOSX/BALLPackage/Library/BALL
	@echo "Copying BALLView"
	@$(CP) BALLView $(BALL_PATH)/MacOSX/BALLPackage/Applications/BALLView.app/Contents/MacOS/
	@echo "Copying QT libraries..."
	@$(CP) $(QT_LIBPATH)/libqt$(QT_MT_SUFFIX).$(SHARED_LIB_SUFFIX) $(BALL_PATH)/MacOSX/BALLPackage/Library/BALL/Library/
	
	
	
