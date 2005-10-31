
!INCLUDE Makefile.lst
!INCLUDE  ..\..\Makefile_options.txt


VIEW_DEFINES	= /D BALL_VIEW_BUILD_DLL /D BALL_QT_HAS_THREADS /D QT_THREAD_SUPPORT  

CC		= $(CC) $(VIEW_INCLUDES) $(VIEW_DEFINES)

LINKDLL		= $(LINKDLL) $(VIEW_LIBS)


UICDIR = "$(INC_DIR)\BALL\VIEW\UIC"


.c.obj:
	$(CC)  $(*).C
	echo A > $(*).obj

.ui.myui:
	echo running uic and moc for $(*B)
	$(QTDIR)\bin\uic -o "$(UICDIR)\$(*B)Data.h"  "$(*).ui"
	$(QTDIR)\bin\uic -o "$(*)Data.C" -impl "$(UICDIR)\$(*B)Data.h" "$(*).ui"
	$(QTDIR)\bin\moc -o "mocfiles\$(*B)_moc.C"  "$(INC_DIR)\BALL\VIEW\DIALOGS\$(*B).h"
	$(QTDIR)\bin\moc -o "mocfiles\$(*B)Data_moc.C"  "$(UICDIR)\$(*B)Data.h"
	echo A >  $(*).myui

.h.mymoc:
	echo running moc for $(*B)
	$(QTDIR)\bin\moc -o "mocfiles\$(*B)_moc.C"  "$(*).h"
	echo A > $(*).mymoc

all: libVIEW.dll

clean:
	cd $(SDIR)\VIEW
	del /S /Q *.obj *._obj *.myui > NUL 2> NUL
	cd $(INC_DIR)
	del /S /Q *.mymoc > NUL 2> NUL
	cd $(MAKEDIR)
	del /Q $(TARGETDIR)\*.* mocfiles\*.* > NUL 2> NUL

libVIEW.dll:  $(UICS) $(MOCS) $(OBJS) 
	$(LINKDLL) /OUT:$(TARGETDIR)\libVIEW.dll 
