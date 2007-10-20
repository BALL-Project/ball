
!INCLUDE  ..\..\Makefile_options.txt
!INCLUDE  Makefile.lst

# options for Compiler
PyBALL_DEFINES = /D QT_DLL /D UNICODE /D QT_THREAD_SUPPORT /D NO_DEBUG /D BALL_QT_HAS_THREADS /D BALL_PYTHON_SUPPORT

CC =  $(CC) $(VIEW_INCLUDES) $(BALLVIEW_DEFINES)

# options for Linker
LINK = $(LINK) $(BALLVIEW_LIBS) $(TARGETDIR)\*.obj

UICDIR = $(SDIR)\Applications\PyBALL

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

all: PyBALL.exe

clean:
	cd "$(UICDIR)"
	$(MYDEL) *.obj *.mymoc *.myui *.myui2 *Data.* *_moc.C aboutDialog.C aboutDialog.h
	cd "$(MAKEDIR)"
	$(MYDEL) $(TARGETDIR)\*.*

PyBALL.exe:  $(MOCS) $(OBJS) 
#compile the resource file:
	rc.exe /i "$(INCLUDE)"\mfc /fo"$(TARGETDIR)\PyBALL.res" PyBALL.rc 
	$(LINK) $(TARGETDIR)\PyBALL.res /OUT:$(TARGETDIR)\PyBALL.exe
	echo.
	echo finished creating PyBALL...
	echo.
