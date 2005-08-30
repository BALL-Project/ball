# -*- Mode: C++; tab-width: 2; -*-
# vi: set ts=2:
#
# $Id: Makefile,v 1.8.2.8 2005/06/19 09:32:45 oliver Exp $
#

EXECUTABLE=BALLView
DIRECTORY=APPLICATIONS/BALLVIEW

all:	$(EXECUTABLE)

clean:	default_clean

include ../../common.mak
-include .Dependencies

BALL_INCLUDES=$(VIEW_INCLUDES) $(PYTHON_INCLUDES)

CPP_SOURCES=\
	aboutDialog.C\
	ballviewTutorial.C\
	ballviewTutorialData.C\
	ballviewDemo.C\
	ballviewDemoData.C\
	icons.C\
	main.C\
	mainframe.C

MOC_SOURCES=\
	aboutDialog_moc.C\
	ballviewTutorial_moc.C\
	ballviewTutorialData_moc.C\
	ballviewDemo_moc.C\
	ballviewDemoData_moc.C\
	mainframe_moc.C

UIC_FILES=\
	aboutDialog.C\
	aboutDialog.h\
	ballviewTutorialData.C\
	ballviewTutorialData.h\
	ballviewDemoData.C

BALLView:	$(OBJECTS)	$(SPECIAL_OBJECTS)
	$(CXX)	$(LDFLAGS) -o $(EXECUTABLE) $(OBJECTS) $(SPECIAL_OBJECTS) $(VIEW_LIBS) $(PYTHON_LIBS)

depend: $(UIC_FILES) $(MOC_SOURCES)

mainframe_moc.C:	mainframe.h
	$(MOC) mainframe.h -o $@

aboutDialog.h: aboutDialog.ui
	$(UIC) aboutDialog.ui > aboutDialog.h
				
aboutDialog_moc.C: aboutDialog.h
	$(MOC) aboutDialog.h -o $@

aboutDialog.C: aboutDialog.h
	$(UIC) -impl aboutDialog.h aboutDialog.ui > aboutDialog.C


ballviewTutorialData.h: ballviewTutorial.ui
	$(UIC) ballviewTutorial.ui > ballviewTutorialData.h
				
ballviewTutorialData.C: ballviewTutorial.h
	$(UIC) -impl ballviewTutorialData.h ballviewTutorial.ui > ballviewTutorialData.C

ballviewTutorial_moc.C: ballviewTutorial.h
	$(MOC) ballviewTutorial.h -o $@

ballviewTutorialData_moc.C: ballviewTutorialData.h
	$(MOC) ballviewTutorialData.h -o $@


ballviewDemoData.h: ballviewDemo.ui
	$(UIC) ballviewDemo.ui > ballviewDemoData.h
				
ballviewDemoData.C: ballviewDemo.h
	$(UIC) -impl ballviewDemoData.h ballviewDemo.ui > ballviewDemoData.C

ballviewDemo_moc.C: ballviewDemo.h
	$(MOC) ballviewDemo.h -o $@

ballviewDemoData_moc.C: ballviewDemoData.h
	$(MOC) ballviewDemoData.h -o $@


clean: clean_moc_uic

clean_moc_uic:
	@$(RM) -f $(UIC_FILES)

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
	
	
	
