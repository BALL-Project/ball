DIRECTORY=MATHS

THISLIB=$(LIBNAME)

MATHS_SUBDIRS=\
	LINALG

FFT_SOURCES=$(BALL_HAS_FFTW:true=FFT1D.C FFT2D.C FFT3D.C)

PARSER_SOURCES= \
	parsedFunction.C\
	parsedFunctionParser.C\
	parsedFunctionLexer.C

CPP_SOURCES= $(FFT_SOURCES)\
	analyticalGeometry.C\
	angle.C\
	box3.C\
	circle3.C\
	function.C\
	line3.C\
	matrix44.C\
	plane3.C\
	piecewiseFunction.C\
	piecewisePolynomial.C\
	quaternion.C\
	simpleBox3.C\
	sphere3.C\
	surface.C\
	vector2.C\
	vector3.C\
	vector4.C\
	parsedFunction.C\
	parsedFunctionLexer.C\
	parsedFunctionParser.C\
	randomNumberGenerator.C

#
# the prefix for the methods defined in the lexer
#
PARSER_PREFIX=parsedFunction

include ../common.mak
-include .Dependencies

depend: $(PARSER_SOURCES) subdirsdepend

default: echodir $(OBJECTS) dirs

collectlib: subdirscollectlib

dirs: dummy
	@$(MAKE) subdirs SUBDIRS="$(MATHS_SUBDIRS)" TARGET=default

subdirs:	dummy
	@set -e; for i in $(SUBDIRS); do cd $$i; if $(MAKE) $(TARGET); then cd .. ; else cd .. ; exit 1 ; fi ; done

subdirsdepend:	dummy
	@echo "" > .Dependencies
	@for i in $(MATHS_SUBDIRS); do ( cd $$i; echo "" >.Dependencies; cd .. ) ; done
	@$(MAKE) subdirs SUBDIRS="$(MATHS_SUBDIRS)" TARGET=depend

subdirsclean:
	@$(MAKE) subdirs SUBDIRS="$(MATHS_SUBDIRS)" TARGET=default_clean

subdirscollectlib:	dummy
	@$(MAKE) subdirs SUBDIRS="$(MATHS_SUBDIRS)" TARGET=collectlib LIBNAME="$(THISLIB)"

default_clean: clean_flex_bison subdirsclean

clean_flex_bison:
	@-$(RM) parsedFunctionParser.C parsedFunctionLexer.C parsedFunctionParser.h *.bak 2>/dev/null

parsedFunctionParser.C: parsedFunctionParser.y
parsedFunctionParser.h: parsedFunctionParser.y

parsedFunctionLexer.o: parsedFunctionLexer.C parsedFunctionParser.C
	$(CXX) $(CXXFLAGS) $(ADD_CXXFLAGS) $(CPP_MODE_FLAGS) $(LIB_CXXFLAGS) $(BALL_INCLUDES) $(ADD_INCLUDES) -c $*.C -o $@

parsedFunctionParser.o: parsedFunctionParser.C
	$(CXX) $(CXXFLAGS) $(ADD_CXXFLAGS) $(CPP_MODE_FLAGS) $(LIB_CXXFLAGS) $(BALL_INCLUDES) $(ADD_INCLUDES) -c $*.C -o $@ || \
	../config/fixbison parsedFunctionParser.C

dummy:
