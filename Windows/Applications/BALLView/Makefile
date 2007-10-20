
!INCLUDE  ..\..\Makefile_options.txt
!INCLUDE  Makefile.lst

# options for Compiler
BALLVIEW_DEFINES = /D QT_DLL /D UNICODE /D QT_THREAD_SUPPORT /D NO_DEBUG /D BALL_QT_HAS_THREADS /D BALL_PYTHON_SUPPORT

CC =  $(CC) $(VIEW_INCLUDES) $(BALLVIEW_DEFINES)

# options for Linker
LINK = $(LINK) $(BALLVIEW_LIBS) $(TARGETDIR)\*.obj

UICDIR = $(SDIR)\Applications\BALLView

.ui.myui:
	echo running uic and moc for $(*B)
	$(QTDIR)\bin\uic -o "$(*R)Data.h"  "$(*).ui"
	echo A >  $(*).myui

.ui.myui2:
	echo running uic and moc for $(*B)
	$(QTDIR)\bin\uic -o "$(*R).h"  "$(*).ui"
	echo A >  $(*).myui2

.h.mymoc:
	echo running moc for $(*B)
	$(QTDIR)\bin\moc -o "$(*R)_moc.C"  "$(*).h"
	echo A > $(*).mymoc

all: BALLView.exe

clean:
	cd "$(UICDIR)"
	$(MYDEL) *.obj *.mymoc *.myui *.myui2 *Data.* *_moc.C aboutDialog.C aboutDialog.h
	cd "$(MAKEDIR)"
	$(MYDEL) $(TARGETDIR)\*.*

BALLView.exe:  $(UICS) $(MOCS) $(OBJS) 
#compile the resource file:
	rc.exe /i "$(INCLUDE)"\mfc /fo"$(TARGETDIR)\BALLView.res" BALLView.rc 
	$(LINK) $(TARGETDIR)\BALLView.res /OUT:$(TARGETDIR)\BALLView.exe
	echo.
	echo finished creating BALLVIEW...
	echo.
