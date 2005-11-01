
!INCLUDE ..\Makefile_options.txt
!INCLUDE ..\..\source\TEST\Makefile.lst

# options for C++ files: no optimizations because for some files this creates an
# error C1053: main_ too large
CC	= cl $(CCOPTIONS) $(INCLUDES) $(DEFINES) $(CC_DEBUG) /TP $(BALL_DEFINES)

LINK	= $(LINK) $(BALL_WIN_DIR)\Libs\libBALL\$(TARGETDIR)\libBALL.lib

o = .obj
EXEEXT = .exe
e = .exe

TESTDIR = $(SDIR)\TEST
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
	cd $(SDIR)\TEST
	del /S /Q *.exe > NUL 2> NUL
	cd $(MAKEDIR)
	del /Q /S $(TARGETDIR)\*.* > NUL 2> NUL
