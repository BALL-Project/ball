DIRECTORY=MATHS

THISLIB=$(LIBNAME)

FFT_SOURCES=$(BALL_HAS_FFTW:true=FFT1D.C FFT2D.C FFT3D.C)

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
	sphere3.C\
	surface.C\
	vector2.C\
	vector3.C\
	vector4.C

include ../common.mak
include .Dependencies
