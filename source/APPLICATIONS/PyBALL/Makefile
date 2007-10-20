# -*- Mode: C++; tab-width: 2; -*-
# vi: set ts=2:
#
#
# Author:
#   Oliver Kohlbacher
#   Andreas Moll
#

DIRECTORY=APPLICATIONS/PyBALL

EXECUTABLE=PyBALL

all:	$(EXECUTABLE)

clean:	default_clean

include ../../common.mak
-include .Dependencies

UIC_DIR=.

CPP_SOURCES=\
	mainframe.C\
	main.C

MOC_SOURCES=\
	mainframe_moc.C

UIC_FILES=

PyBALL:	$(OBJECTS)	$(SPECIAL_OBJECTS)
	$(CXX)	$(LDFLAGS) -o $(EXECUTABLE) $(OBJECTS) $(SPECIAL_OBJECTS) $(BALL_LIBS) $(PYTHON_LIBS)

depend: $(UIC_FILES) $(MOC_SOURCES)

mainframe_moc.C:	mainframe.h
	$(MOC) mainframe.h -o $@

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
	
	
	
