
TARGET = BALLView

include ..\..\Makefile_options.txt

BALLVIEW_PATH = "$(SDIR)\Applications\BALLView"

include Makefile.lst

########################
# options for Compiler #
########################

BALLVIEW_DEFINES = /D QT_DLL /D UNICODE /D QT_THREAD_SUPPORT /D NO_DEBUG /D BALL_QT_HAS_THREADS /D BALL_PYTHON_SUPPORT $(DEFINES)
BALLVIEW_INCLUDES = $(INCLUDES) $(VIEW_INCLUDES)
BALLVIEW_LIBS = $(VIEW_LIBS) $(BALL_WIN_DIR)\Libs\libVIEW\$(TARGETDIR)\libVIEW.lib

# options for C++ files
CC =  cl $(CCOPTIONS) $(BALLVIEW_INCLUDES)  $(BALLVIEW_DEFINES) $(OPT) /TP

########################
# options for Linker   #
########################

LINK = link  -nologo /OUT:$(TARGETDIR)\BALLView.exe $(BALLVIEW_LIBS) $(TARGETDIR)\*.obj

.SUFFIXES: .ui .moc .h .myui

UICDIR = $(SDIR)\Applications\BALLView

.c.obj:
	$(CC)  $(*).C
	echo A > $(*).obj

.ui.myui:
	echo running uic and moc for $(*B)
	$(QTDIR)\bin\uic -o "$(*R)Data.h"  "$(*).ui"
	$(QTDIR)\bin\uic -o "$(*R)Data.C" -impl "$(*R)Data.h" "$(*).ui"
	echo A >  $(*).myui

.h.mymoc:
	echo running moc for $(*B)
	$(QTDIR)\bin\moc -o "$(*R)_moc.C"  "$(*).h"
	echo A > $(*).mymoc

all: $(TARGET).exe

clean:
	$(BINPATH)\rm.exe -f  $(TARGETDIR)/*.* $(OBJS) $(UICS) $(MOCS)

$(TARGET).exe:  $(UICS) $(MOCS) $(OBJS) 
	rc.exe /fo"$(TARGETDIR)\BALLView.res" BALLView.rc
	$(LINK) $(TARGETDIR)\BALLView.res
