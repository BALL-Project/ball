// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//

#ifndef BALL_MATHS_FFTWCOMMON_H
#define BALL_MATHS_FFTWCOMMON_H

#ifndef BALL_COMMON_H
# include <BALL/common.h>
#endif

#include <fftw3.h>

namespace BALL
{
	struct BALL_EXPORT DoubleTraits
	{
		typedef double ComplexPrecision;
		typedef fftw_plan FftwPlan;
	};
	
	struct BALL_EXPORT FloatTraits
	{
		typedef float ComplexPrecision;
		typedef fftwf_plan FftwPlan;
	};
	
	struct BALL_EXPORT LongDoubleTraits
	{
		typedef long double ComplexPrecision;
		typedef fftwl_plan FftwPlan;
	};
}

#endif
