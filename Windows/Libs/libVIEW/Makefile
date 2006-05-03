
!INCLUDE Makefile.lst
!INCLUDE ..\..\Makefile_options.txt


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
	$(QTDIR)\bin\moc -o "mocfiles\$(*B)_moc.C"  "$(INC_DIR)\BALL\VIEW\DIALOGS\$(*B).h"
	echo A >  $(*).myui

.h.mymoc:
	echo running moc for $(*B)
	$(QTDIR)\bin\moc -o "mocfiles\$(*B)_moc.C"  "$(*).h"
	echo A > $(*).mymoc

all: libVIEW.dll

clean:
	cd "$(SDIR)\VIEW"
	$(MYDEL_R) *.obj  *._obj  *.mymoc *Data.C *.myui
	cd "$(INC_DIR)\BALL\VIEW"
	$(MYDEL_R) *.mymoc
	cd "$(MAKEDIR)"
	$(MYDEL)   $(TARGETDIR)\*.*  mocfiles\*.*

libVIEW.dll:  $(UICS) $(MOCS) $(OBJS)
	$(LINKDLL)  /SUBSYSTEM:WINDOWS /OUT:$(TARGETDIR)\libVIEW.dll 
