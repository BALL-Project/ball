// $Id: complex.C,v 1.3 2002/10/12 17:06:57 oliver Exp $

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
