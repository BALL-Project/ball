// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: complex.h,v 1.20 2004/02/18 18:19:04 anhi Exp $

#ifndef BALL_MATHS_COMPLEX_H
#define BALL_MATHS_COMPLEX_H

#ifndef BALL_COMMON_H
# include <BALL/common.h>
#endif

#include <complex>

namespace BALL 
{
	typedef std::complex<BALL_COMPLEX_PRECISION> Complex;
} // namespace BALL

#endif // BALL_MATHS_COMPLEX_H
