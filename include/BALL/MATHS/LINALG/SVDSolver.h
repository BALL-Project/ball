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

namespace BALL {
	// explicit instantiation to save the compiler some work (and to *make* it work on Windows)
	template class BALL_EXPORT SVDSolver<float, StandardTraits>;
	template class BALL_EXPORT SVDSolver<double, StandardTraits>;
	template class BALL_EXPORT SVDSolver<ComplexFloat, StandardTraits>;
	template class BALL_EXPORT SVDSolver<ComplexDouble, StandardTraits>;
}

#endif