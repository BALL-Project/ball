// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: complex.C,v 1.4.2.1 2003/02/05 15:32:53 anker Exp $

#include <BALL/MATHS/complex.h>

namespace BALL
{
	#ifdef BALL_HAS_FFTW
	std::istream& operator >> (std::istream& s, FFTW_COMPLEX& cpx)
		throw()
	{
		return s >> cpx.re >> cpx.im;
	}

	std::ostream& operator << (std::ostream& s, const FFTW_COMPLEX& cpx)
		throw()
	{
		
		return s << cpx.re << " " << cpx.im;
	}
	#endif

}
