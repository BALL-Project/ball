
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
	$(QTDIR)\bin\uic -o "$(*R)Data.C" -impl "$(*R)Data.h" "$(*).ui"
	echo A >  $(*).myui

.h.mymoc:
	echo running moc for $(*B)
	$(QTDIR)\bin\moc -o "$(*R)_moc.C"  "$(*).h"
	echo A > $(*).mymoc

all: BALLView.exe

clean:
	$(BINPATH)\rm.exe -f  $(TARGETDIR)/*.* $(OBJS) $(UICS) $(MOCS)

BALLView.exe:  $(UICS) $(MOCS) $(OBJS) 
#compile the resource file:
	rc.exe /fo"$(TARGETDIR)\BALLView.res" BALLView.rc
	$(LINK) $(TARGETDIR)\BALLView.res /OUT:$(TARGETDIR)\BALLView.exe
