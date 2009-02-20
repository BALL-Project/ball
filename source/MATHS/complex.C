// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: complex.C,v 1.6 2005/02/02 10:50:49 anhi Exp $

#include <BALL/MATHS/complex.h>


// AR: obsolete now by using of the C++-datatype complex<.> and new FFTW3
//     Can be removed in future
namespace BALL
{
	#ifdef BALL_HAS_FFTW
	std::istream& operator >> (std::istream& s, fftw_complex& cpx)
	{
		return s >> cpx[0] >> cpx[1];
	}

	std::ostream& operator << (std::ostream& s, const fftw_complex& cpx)
	{	
		return s << cpx[0] << " " << cpx[1];
	}
	#endif

}
