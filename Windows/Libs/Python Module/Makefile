
TARGET = BALL

include Makefile.lst
include ..\..\Makefile_options.txt

.SUFFIXES: .sip .mysip

EXTENSION_DIR = $(SDIR)\PYTHON\Extensions
########################
# options for Compiler #
########################

# options for C++ files
CC =  cl $(CCOPTIONS) $(INCLUDES) $(VIEW_INCLUDES) $(DEFINES) $(VIEW_DEFINES) $(OPT) /TP

########################
# options for Linker   #
########################

LINK = link  $(LINKOPTIONS) $(BALLVIEW_LIBS) $(TARGETDIR)\*.obj 

all: $(EXTENSION_DIR)\BALL.mysip $(TARGET).dll

clean:
	$(BINPATH)\rm.exe -f  $(TARGETDIR)/*.* $(EXTENSION_DIR)/BALL.mysip *.C *.h *.sip *.obj

$(TARGET).dll:   $(OBJS) 
	$(LINK)

.sip.mysip:
	echo running sip for $(*B)
	copy $(EXTENSION_DIR)\*.sip	.
	copy $(EXTENSION_DIR)\*.C	.
	$(BALL_CONTRIB)\sip\sipgen\sip.exe -e -s ".C"  -c .  $(*).sip
	echo A > $@
	

.c.obj:
	$(CC)  $(*).C
	echo A > $(*).obj