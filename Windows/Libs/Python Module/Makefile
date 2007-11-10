
!INCLUDE  Makefile.lst
!INCLUDE  ..\..\Makefile_options.txt

EXTENSION_DIR = $(SDIR)\PYTHON\Extensions

# options for C++ files: no optimization!
CC = cl $(CCOPTIONS) $(INCLUDES) $(DEFINES) $(CC_DEBUG) /TP $(VIEW_INCLUDES) $(VIEW_DEFINES)

# options for Linker
LINKDLL = $(LINKDLL) $(BALLVIEW_LIBS) /EXPORT:initBALL

all: $(EXTENSION_DIR)\BALL.mysip BALL.dll

clean:
	move BALLmodule.C .. > NUL 2> NUL
	$(MYDEL) "$(EXTENSION_DIR)\BALL.mysip"  "$(EXTENSION_DIR)\sipBALL*.*"  *.C *.h *.sip *.obj  $(TARGETDIR)\*.* 
	move ..\BALLmodule.C . > NUL 2> NUL

BALL.dll:   $(OBJS) 
	$(LINKDLL) /OUT:$(TARGETDIR)\BALL.dll 

.sip.mysip:
	echo running sip for $(*B)
	copy $(EXTENSION_DIR)\*.sip	. > NUL
	copy $(EXTENSION_DIR)\*.C	. > NUL
	$(BALL_CONTRIB)\sip\sipgen\sip.exe -e -s ".C"  -c .  $(*).sip
	$(PYTHON_DIR)\python.exe collectCFiles.py > Makefile.lst
	echo A > $@
	

.c.obj:
	$(CC)  $(*).C
	echo A > $(*).obj
