include ../Make.rules.in

DIRECTORY=MATHS

if BALL_HAS_FFTW
FFT_SRC=FFT1D.C FFT2D.C FFT3D.C
else
FFT_SRC=
endif

_objects_LINK= ../config/genobjects.sh $(DIRECTORY)

BUILT_SOURCES=parsedFunctionParser.C parsedFunctionLexer.C parsedFunctionParser.h

parsedFunctionParser.C: parsedFunctionParser.y
parsedFunctionParser.h: parsedFunctionParser.y

CURRENTDIR_SOURCES=complex.C\
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
	sphere3.C\
	surface.C\
	vector2.C\
	vector3.C\
	vector4.C\
	parsedFunction.C\
	parsedFunctionLexer.C\
	parsedFunctionParser.C\
	$(FFT_SRC)

clean-local:
	$(RM) -f parsedFunctionParser.C parsedFunctionLexer.C parsedFunctionParser.h

#
# the prefix for the methods defined in the lexer
#
PARSER_PREFIX=parsedFunction
