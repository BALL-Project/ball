#ifndef BALL_LINALG_SVDSOLVER_H
#define BALL_LINALG_SVDSOLVER_H

#include <BALL/MATHS/LINALG/matrix.h>
#include <BALL/MATHS/LINALG/vector.h>

namespace BALL {
	class StandardTraits;

	template <class valuetype, class mtraits=StandardTraits>
	class SVDSolver;
}

#include <BALL/MATHS/LINALG/SVDSolver.ih>
#include <BALL/MATHS/LINALG/SVDSolver.iC>

#endif
