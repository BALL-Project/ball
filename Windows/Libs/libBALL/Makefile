
!INCLUDE Makefile.lst
!INCLUDE ..\..\Makefile_options.txt

########################
# options for Compiler #
########################

BALL_DEFINES = /D _MBCS /D HAVE_STRING_H /D BALL_BUILD_DLL

# options for C++ files
CC		= $(CC) $(BALL_DEFINES)

# options for C files
CCC		= cl $(CCOPTIONS) $(INCLUDES) $(DEFINES) $(BALL_DEFINES) $(CC_OPT) $(CC_DEBUG) /TC

# options for C++ without optimization
NOOPT_CC	= cl $(CCOPTIONS) $(INCLUDES) $(DEFINES) $(BALL_DEFINES) $(CC_DEBUG) /TP


FLEX = $(BINPATH)\flex

# bison will crash if not run with absolute path!!!
BISON = $(BINPATH)\bison

all: flex_bison regex libBALL.dll

clean:
	cd "$(SDIR)"
	$(MYDEL_R) *.obj *._obj
	cd "$(MAKEDIR)"
	$(MYDEL) $(TARGETDIR)\*.*

libBALL.dll:   $(NO_OPTIMIZATION_OBJS)  $(OBJS) $(OBJS2)
	$(LINKDLL) /OUT:$(TARGETDIR)\libBALL.dll 

.c.obj:
	$(CC)  $(*).C
	echo A > $(*).obj


# rule to create non optimized C++ files:
.c._obj:
	$(NOOPT_CC)  $(*).C
	echo A > $(*)._obj

flex_bison: 
	echo ------------------------
	echo running flex and bison
	set BISON_PKGDATADIR=$(BINPATH)
	set PATH=%PATH%;$(BINPATH)
	set M4=$(BINPATH)\M4.exe
	$(FLEX) -PExpressionParser -o"$(SDIR)\KERNEL\expressionParserLexer.C" $(SDIR)\KERNEL\expressionParserLexer.l
	$(FLEX) -PSmilesParser -o"$(SDIR)\STRUCTURE\smilesParserLexer.C" $(SDIR)\STRUCTURE\smilesParserLexer.l
	$(FLEX) -PSmartsParser -o"$(SDIR)\STRUCTURE\smartsParserLexer.C" $(SDIR)\STRUCTURE\smartsParserLexer.l
	$(FLEX) -PparsedFunction -o"$(SDIR)\MATHS\parsedFunctionLexer.C" $(SDIR)\MATHS\parsedFunctionLexer.l
	$(BISON) -y -p ExpressionParser -d $(SDIR)\KERNEL\expressionParserParser.y -o $(SDIR)\KERNEL\expressionParserParser.C
	$(BISON)  -y -p parsedFunction -d $(SDIR)\MATHS\parsedFunctionParser.y -o $(SDIR)\MATHS\parsedFunctionParser.C
	$(BISON)  -y -p SmilesParser -d $(SDIR)\STRUCTURE\smilesParserParser.y -o $(SDIR)\STRUCTURE\smilesParserParser.C
	$(BISON)  -y -p SmartsParser -d $(SDIR)\STRUCTURE\smartsParserParser.y -o $(SDIR)\STRUCTURE\smartsParserParser.C
	echo ------------------------
	echo compiling
	echo ------------------------

regex:
	$(CCC) ..\..\Contrib\regex-0.12\regex.C /D __STDC__ /D REGEX_MALLOC