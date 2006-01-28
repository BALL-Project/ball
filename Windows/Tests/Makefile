
!INCLUDE ..\Makefile_options.txt

TESTDIR = ..\..\source\TEST

PREFIX = ..\..\source\TEST\\

!INCLUDE $(TESTDIR)\Makefile.lst

# options for C++ files: no optimizations because for some files this creates an
# error C1053: main_ too large
CC	= cl $(CCOPTIONS) $(INCLUDES) $(DEFINES) $(CC_DEBUG) /TP $(BALL_DEFINES)

LINK	= $(LINK) $(BALLVIEW_LIBS)

o = .obj
EXEEXT = .exe
e = .exe

INDIR = $(SDIR)\TEST
PREFIX = $(TESTDIR)\\

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

all:	$(EXECUTABLES)

clean: 
	$(MYDEL) "$(SDIR)\TEST"\*.exe
	$(MYDEL) $(TARGETDIR)\*.*
