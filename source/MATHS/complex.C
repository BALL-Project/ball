// $Id: complex.C,v 1.2.2.1 2002/09/08 18:46:38 anhi Exp $

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
