DIRECTORY=MATHS

THISLIB=$(LIBNAME)

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
	complex.C\
	function.C\
	line3.C\
	matrix44.C\
	plane3.C\
	piecewiseFunction.C\
	piecewisePolynomial.C\
	quaternion.C\
	sphere3.C\
	surface.C\
	vector2.C\
	vector3.C\
	vector4.C\
	parsedFunction.C\
	parsedFunctionLexer.C\
	parsedFunctionParser.C

#
# the prefix for the methods defined in the lexer
#
PARSER_PREFIX=parsedFunction

include ../common.mak
include .Dependencies

depend: $(PARSER_SOURCES)

default_clean: clean_flex_bison

clean_flex_bison:
	@-$(RM) parsedFunctionParser.C parsedFunctionLexer.C parsedFunctionParser.h

parsedFunctionParser.C: parsedFunctionParser.y
parsedFunctionParser.h: parsedFunctionParser.y

parsedFunctionLexer.o: parsedFunctionLexer.C parsedFunctionParser.C
	$(CXX) $(CXXFLAGS) $(ADD_CXXFLAGS) $(CPP_MODE_FLAGS) $(LIB_CXXFLAGS) $(BALL_INCLUDES) $(ADD_INCLUDES) -c $*.C -o $@

parsedFunctionParser.o: parsedFunctionParser.C
	$(CXX) $(CXXFLAGS) $(ADD_CXXFLAGS) $(CPP_MODE_FLAGS) $(LIB_CXXFLAGS) $(BALL_INCLUDES) $(ADD_INCLUDES) -c $*.C -o $@
