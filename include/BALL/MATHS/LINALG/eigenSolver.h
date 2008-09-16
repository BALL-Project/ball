#ifndef BALL_LINALG_EIGENSOLVER_H
#define BALL_LINALG_EIGENSOLVER_H

#include <BALL/MATHS/LINALG/matrix.h>
#include <BALL/MATHS/LINALG/vector.h>

// TODO: make setup allocate work space on demand!
namespace BALL {
	class StandardTraits;

	template <class valuetype, class mtraits=StandardTraits>
	class EigenSolver;
}

#include <BALL/MATHS/LINALG/eigenSolver.ih>
#include <BALL/MATHS/LINALG/eigenSolver.iC>

#endif
