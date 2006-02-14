// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: complex.h,v 1.21.6.1 2006/02/14 15:01:01 amoll Exp $

#ifndef BALL_MATHS_COMPLEX_H
#define BALL_MATHS_COMPLEX_H

#ifndef BALL_COMMON_H
# include <BALL/common.h>
#endif

#include <complex>

#include <fftw3.h>

namespace BALL 
{
	typedef std::complex<BALL_COMPLEX_PRECISION> Complex;
} // namespace BALL

#endif // BALL_MATHS_COMPLEX_H
