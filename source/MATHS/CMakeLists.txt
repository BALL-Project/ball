set(path MATHS)


SET(YACC_SOURCES "")
SET(YACC_SOURCES 
	parsedFunctionParser.y
)

foreach(i ${YACC_SOURCES})
	wrap_YACC(BALL ${path} ${i})
endforeach(i)


SET(FLEX_SOURCES "")
SET(FLEX_SOURCES 
	parsedFunctionLexer.l
)

foreach(i ${FLEX_SOURCES})
	wrap_FLEX(BALL ${path} ${i})
endforeach(i)


list(APPEND BALL_SOURCES 
	MATHS/analyticalGeometry.C
	MATHS/angle.C
	MATHS/box3.C
	MATHS/circle3.C
	MATHS/common.C
	MATHS/function.C
	MATHS/line3.C
	MATHS/matrix44.C
	MATHS/parsedFunction.C
	MATHS/piecewiseFunction.C
	MATHS/piecewisePolynomial.C
	MATHS/plane3.C
	MATHS/quaternion.C
	MATHS/randomNumberGenerator.C
	MATHS/simpleBox3.C
	MATHS/sphere3.C
	MATHS/surface.C
	MATHS/vector2.C
	MATHS/vector3.C
	MATHS/vector4.C
)

if(USE_FFTW)
	list(APPEND BALL_SOURCES 
		MATHS/complex.C
		MATHS/FFT1D.C
		MATHS/FFT2D.C
		MATHS/FFT3D.C
		)
endif(USE_FFTW)


include(MATHS/LINALG/CMakeLists.txt)
