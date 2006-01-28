
!INCLUDE ..\Makefile_options.txt
!INCLUDE ..\..\source\Benchmarks\Makefile.lst

# options for C++ files: 
CC	= $(CC) $(BALL_DEFINES)

LINK	= $(LINK) $(BALL_WIN_DIR)\Libs\libBALL\$(TARGETDIR)\libBALL.lib

o = .obj
EXEEXT = .exe
e = .exe

BENCHMARKDIR = $(SDIR)\Benchmarks
PREFIX = $(BENCHMARKDIR)\\

.c.exe:
	$(CC) $(*).C
	$(LINK) $(TARGETDIR)\$(*B).obj 
	move $(*B).exe $(TARGETDIR)
	echo A > $(BENCHMARKDIR)\$(*B).exe


all:	$(EXECUTABLES)

clean: 
	$(MYDEL) "$(BENCHMARKDIR)"\*.exe
	$(MYDEL) $(TARGETDIR)\*.*
