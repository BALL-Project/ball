
!INCLUDE  Makefile.lst
!INCLUDE  ..\..\Makefile_options.txt

EXTENSION_DIR = $(SDIR)\PYTHON\Extensions

# options for C++ files
CC =  $(CC) $(VIEW_INCLUDES) $(VIEW_DEFINES)

# options for Linker
LINKDLL = $(LINKDLL) $(BALLVIEW_LIBS) /EXPORT:initBALL

all: $(EXTENSION_DIR)\BALL.mysip BALL.dll

clean:
	move BALLmodule.C .. > NUL 2> NUL
	cd "$(EXTENSION_DIR)"
	$(MYDEL) BALL.mysip sipBALL*.*
	cd "$(MAKEDIR)"
	$(MYDEL) *.C *.h *.sip *.obj
	$(MYDEL) (TARGETDIR)\*.*
	move ..\BALLmodule.C . > NUL 2> NUL

BALL.dll:   $(OBJS) 
	$(LINKDLL) /OUT:$(TARGETDIR)\BALL.dll 

.sip.mysip:
	echo running sip for $(*B)
	copy $(EXTENSION_DIR)\*.sip	.
	copy $(EXTENSION_DIR)\*.C	.
	$(BALL_CONTRIB)\sip\sipgen\sip.exe -e -s ".C"  -c .  $(*).sip
	$(PYTHONDIR)\python.exe collectCFiles.py > Makefile.lst
	echo A > $@
	

.c.obj:
	$(CC)  $(*).C
	echo A > $(*).obj
