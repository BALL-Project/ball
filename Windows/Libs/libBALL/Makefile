
TARGETDIR = TEST
TARGET = libBALL
SDIR= ..\..\..\source

########################
# options for Compiler #
########################

OPT = /O2 /Ot /GA

INCLUDES = /I "..\..\..\include" /I "..\..\include" /I "..\..\Contrib\regex-0.12" /I "..\..\Contrib\ONC_RPC" /I "$(PYTHONDIR)\include"

DEFINES = /D WIN32 /D _WINDOWS /D _USRDLL /D YY_NEVER_INTERACTIVE /D BALL_BUILD_DLL /D _WINDLL  /U ERROR /U IN /U OUT

BALL_DEFINES = /D _MBCS /D HAVE_STRING_H /D __STDC__ /D REGEX_MALLOC

CCOPTIONS = /FD /EHsc /MD /GR /W0 /nologo -c /Zm800 /Fo"$(TARGETDIR)\\"

# options for C++ files
CC		= cl $(CCOPTIONS) $(INCLUDES) $(DEFINES) $(BALL_DEFINES) $(OPT) /TP

# options for C files
CCC		= cl $(CCOPTIONS) $(INCLUDES) $(DEFINES) $(BALL_DEFINES) $(OPT) /TC

# options for C++ without optimization
NOOPT_CC	= cl $(CCOPTIONS) $(INCLUDES) $(DEFINES) $(BALL_DEFINES) /TP

########################
# options for Linker   #
########################

LIBS = $(PYTHONDIR)\libs\python23.lib ..\..\Contrib\ONC_RPC\librpc\oncrpc.lib imm32.lib winmm.lib ws2_32.lib  kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib

LINKOPTIONS = -nologo /OUT:$(TARGETDIR)\$(TARGET).dll /NOLOGO /DLL $(LIBS)  /FORCE:UNRESOLVED

LINK = link  $(LINKOPTIONS) $(TARGETDIR)\*.obj

BINPATH = $(MAKEDIR)\..\..\Contrib\bin

FLEX = $(BINPATH)\flex

# bison will crash if not run with absolute path!!!
BISON = $(BINPATH)\bison

include Makefile.lst

all: flex_bison regex $(TARGET).dll


clean:
	$(BINPATH)\rm.exe -fi  asd $(TARGETDIR)\\*.* $(OBJS) $(NO_OPTIMIZATION_OBJS)

$(TARGET).dll:   $(NO_OPTIMIZATION_OBJS)  $(OBJS) 
	$(LINK)

.c.obj:
	$(CC)  $(*).C
	echo A > $(*).obj

.c._obj:
	$(NOOPT_CC)  $(*).C
	echo A > $(*)._obj

flex_bison: 
	echo ------------------------
	echo running flex and bison
	$(FLEX) -PExpressionParser -o"$(SDIR)\KERNEL\expressionParserLexer.C" $(SDIR)\KERNEL\expressionParserLexer.l
	$(FLEX) -PSmilesParser -o"$(SDIR)\STRUCTURE\smilesParserLexer.C" $(SDIR)\STRUCTURE\smilesParserLexer.l
	$(FLEX) -PparsedFunction -o"$(SDIR)\MATHS\parsedFunctionLexer.C" $(SDIR)\MATHS\parsedFunctionLexer.l
	$(BISON) -y -p ExpressionParser -d $(SDIR)\KERNEL\expressionParserParser.y -o $(SDIR)\KERNEL\expressionParserParser.C
	$(BISON)  -y -p parsedFunction -d $(SDIR)\MATHS\parsedFunctionParser.y -o $(SDIR)\MATHS\parsedFunctionParser.C
	$(BISON)  -y -p SmilesParser -d $(SDIR)\STRUCTURE\smilesParserParser.y -o $(SDIR)\STRUCTURE\smilesParserParser.C
	echo ------------------------
	echo compiling
	echo ------------------------

regex:
	$(CCC) ..\..\Contrib\regex-0.12\regex.C