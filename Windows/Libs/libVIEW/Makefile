
TARGETDIR = TEST
TARGET = libVIEW


########################
# options for Compiler #
########################

OPT = /O2 /Ot /GA /Og

INCLUDES = /I "..\..\..\include" /I "..\..\include" /I "..\..\Contrib\regex-0.12" /I "..\..\Contrib\ONC_RPC" /I "$(PYTHONDIR)\include"

VIEW_INCLUDES = /I "$(QTDIR)\include"

DEFINES = /D WIN32 /D _WINDOWS /D _USRDLL /D YY_NEVER_INTERACTIVE /D BALL_BUILD_DLL /D _WINDLL /U ERROR /U IN /U OUT

VIEW_DEFINES = /D BALL_VIEW_BUILD_DLL /D BALL_QT_HAS_THREADS /D QT_THREAD_SUPPORT  

CCOPTIONS = /FD /EHsc /MD /GR /W0 /nologo -c /Zm800 /Fo"$(TARGETDIR)\\"

# options for C++ files
CC =  cl $(CCOPTIONS) $(INCLUDES) $(VIEW_INCLUDES) $(DEFINES) $(VIEW_DEFINES) $(OPT) /TP

########################
# options for Linker   #
########################

LIBS = $(PYTHONDIR)\libs\python23.lib ..\..\Contrib\ONC_RPC\librpc\oncrpc.lib imm32.lib winmm.lib ws2_32.lib  kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib

VIEW_LIBS = $(LIBS) "$(MSVCDIR)\PlatformSDK\Lib\WS2_32.Lib" $(QTDIR)\lib\qt-mt335.lib $(QTDIR)\lib\qtmain.lib opengl32.lib glu32.lib ..\libBALL\$(TARGETDIR)\libBALL.lib

LINKOPTIONS = -nologo /OUT:$(TARGETDIR)\$(TARGET).dll /NOLOGO /DLL $(VIEW_LIBS) /FORCE:UNRESOLVED

LINK = link  $(LINKOPTIONS) $(TARGETDIR)\*.obj

UICDIR = ..\..\..\include\BALL\VIEW\UIC

.SUFFIXES: .ui .moc .h .myui

.c.obj:
	$(CC)  $(*).C
	echo A > $(*).obj

.ui.myui:
	echo running uic and moc for $(*B)
	$(QTDIR)\bin\uic -o "$(UICDIR)\$(*B)Data.h"  "$(*).ui"
	$(QTDIR)\bin\uic -o "$(*)Data.C" -impl "$(UICDIR)\$(*B)Data.h" "$(*).ui"
	$(QTDIR)\bin\moc -o "mocfiles\$(*B)_moc.C"  "..\..\..\include\BALL\VIEW\DIALOGS\$(*B).h"
	$(QTDIR)\bin\moc -o "mocfiles\$(*B)Data_moc.C"  "$(UICDIR)\$(*B)Data.h"
	echo A >  $(*).myui

.h.mymoc:
	echo running moc for $(*B)
	$(QTDIR)\bin\moc -o "mocfiles\$(*B)_moc.C"  "$(*).h"
	echo A > $(*).mymoc

include Makefile.lst


all: $(TARGET).dll


clean:
	del $(TARGETDIR)\$(TARGET).lib $(TARGETDIR)\$(TARGET).dll $(TARGETDIR)\$(TARGET).exp $(TARGETDIR)\*.pdb $(TARGETDIR)\*.obj $(OBJS) $(NO_OPTIMIZATION_OBJS)


$(TARGET).dll:  $(UICS) $(MOCS) $(OBJS) 
	$(LINK)
