
!INCLUDE  Makefile.lst
!INCLUDE  ..\..\Makefile_options.txt

EXTENSION_DIR = $(SDIR)\PYTHON\Extensions

# options for C++ files
CC =  $(CC) $(VIEW_INCLUDES) $(VIEW_DEFINES)

# options for Linker
LINKDLL = $(LINKDLL) $(BALLVIEW_LIBS)

all: $(EXTENSION_DIR)\BALL.mysip BALL.dll

clean:
	del /S /Q *.C *.h *.sip > NUL 2> NUL
	del /Q /S $(TARGETDIR)\*.* > NUL 2> NUL
	del /Q /S *.obj > NUL 2> NUL
	del /Q /S $(EXTENSION_DIR)\BALL.mysip > NUL 2> NUL


BALL.dll:   $(OBJS) 
	$(LINKDLL) /OUT:$(TARGETDIR)\BALL.dll 

.sip.mysip:
	echo running sip for $(*B)
	copy $(EXTENSION_DIR)\*.sip	.
	copy $(EXTENSION_DIR)\*.C	.
	$(BALL_CONTRIB)\sip\sipgen\sip.exe -e -s ".C"  -c .  $(*).sip
	echo A > $@
	

.c.obj:
	$(CC)  $(*).C
	echo A > $(*).obj
