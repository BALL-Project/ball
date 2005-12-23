
!INCLUDE ..\Makefile_options.txt

# options for C++ files: no optimizations because for some files this creates an
# error C1053: main_ too large
CC	= cl $(CCOPTIONS) $(INCLUDES) $(VIEW_INCLUDES) $(DEFINES) $(CC_DEBUG) /TP $(BALL_DEFINES)

LINK	= $(LINK) $(BALLVIEW_LIBS)

o = .obj
EXEEXT = .exe
e = .exe

INDIR = $(SDIR)\TUTORIAL\

.c.exe:
	$(CC) $(*).C
	$(LINK) $(TARGETDIR)\$(*B).obj 
	move $(*B).exe $(TARGETDIR)
	echo A > $(TESTDIR)\$(*B).exe

EXECUTABLES = \
	$(INDIR)\bounding_box.exe  \
	$(INDIR)\string_tutorial.exe  \
	$(INDIR)\tutorial1.exe  \
	$(INDIR)\tutorial2.exe  \
	$(INDIR)\tutorial3.exe 

all:	$(EXECUTABLES)

clean: 
	$(MYDEL) "$(INDIR)\*.exe"
	$(MYDEL) $(TARGETDIR)\*.*
