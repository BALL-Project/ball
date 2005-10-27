
include ..\Makefile_options.txt

include ..\..\source\TEST\Makefile.lst

# options for C++ files
CC		= cl $(CCOPTIONS) $(INCLUDES) $(DEFINES) $(BALL_DEFINES) $(OPT) /TP

LINK = link  $(LINKOPTIONS_EXE) $(BALL_WIN_DIR)\Libs\libBALL\$(TARGETDIR)\libBALL.lib

o = .obj
EXEEXT = .exe
e = .exe

TESTDIR = $(SDIR)\TEST
PREFIX = $(TESTDIR)\\

#.obj.exe:
#	$(LD) $(LFLAGS) $*.obj  $(BALL_LIB) $(XLIB) $(LDOPTS)

#.c.obj:
#	$(CC) $(CFLAGS) $(DFLAGS) $(IFLAGS) $(INCL) $(*).C

.c.exe:
	$(CC) $(*).C
	$(LINK) $(TARGETDIR)\$(*B).obj 
	move $(*B).exe $(TARGETDIR)
	echo A > $(TESTDIR)\$(*B).exe


EXECUTABLES = \
	$(EXECUTABLES_BASIC)\
	$(EXECUTABLES_SYSTEM)\
	$(EXECUTABLES_MATHS)\
	$(EXECUTABLES_DATATYPES)\
	$(EXECUTABLES_CONCEPTS)\
	$(EXECUTABLES_KERNEL)\
	$(EXECUTABLES_FORMATS)\
	$(EXECUTABLES_MOLMEC)\
	$(EXECUTABLES_NMR)\
	$(EXECUTABLES_ENERGY)\
	$(EXECUTABLES_STRUCTURE)\
	$(EXECUTABLES_QSAR)\
	$(EXECUTABLES_SOLVATION)\
	$(XDR_DEPENDENT_EXECUTABLES)

# default target

all:	$(EXECUTABLES)

clean: 
	$(BINPATH)\rm.exe -f  $(TARGETDIR)/*.* $(EXECUTABLES)
