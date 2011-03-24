#INCLUDE(source/MATHS/LINALG/sources.cmake)

### list all filenames of the directory here ###
SET(SOURCES_LIST
	analyticalGeometry.C
	angle.C
	box3.C
	circle3.C
	cubicSpline1D.C
	cubicSpline2D.C
	function.C
	line3.C
	matrix44.C
	parsedFunction.C
	plane3.C
	piecewiseFunction.C
	piecewisePolynomial.C
	quaternion.C
	randomNumberGenerator.C
	simpleBox3.C
	sphere3.C
	surface.C
	vector2.C
	vector3.C
	vector4.C
)

IF (BALL_HAS_FFTW)
	SET(SOURCES_LIST ${SOURCES_LIST} FFT1D.C FFT2D.C FFT3D.C)
ENDIF()

ADD_BALL_SOURCES("MATHS" "${SOURCES_LIST}")

ADD_BALL_PARSER_LEXER("MATHS" "parsedFunction" "ParsedFunction")
