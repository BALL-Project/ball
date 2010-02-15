// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: complex.h,v 1.22 2006/01/03 18:15:47 anhi Exp $

#ifndef BALL_MATHS_COMPLEX_H
#define BALL_MATHS_COMPLEX_H

#ifndef BALL_COMMON_H
# include <BALL/common.h>
#endif

#include <complex>

#ifdef BALL_HAS_FFTW_H
	#include <fftw3.h>
#endif

namespace BALL 
{
	typedef std::complex<BALL_COMPLEX_PRECISION> Complex;
} // namespace BALL

#endif // BALL_MATHS_COMPLEX_H
