#ifndef BALL_LINALG_MATRIXINVERTER_H
#define BALL_LINALG_MATRIXINVERTER_H

#include <BALL/MATHS/LINALG/matrix.h>

namespace BALL {
	class StandardTraits;

	template <class valuetype, class mtraits=StandardTraits>
	class MatrixInverter;
}

#include <BALL/MATHS/LINALG/matrixInverter.ih>
#include <BALL/MATHS/LINALG/matrixInverter.iC>

namespace BALL {
	// explicit instantiation to save the compiler some work (and to *make* it work on Windows)
	template class BALL_EXPORT MatrixInverter<float, StandardTraits>;
	template class BALL_EXPORT MatrixInverter<double, StandardTraits>;
	//template class BALL_EXPORT MatrixInverter<ComplexFloat, StandardTraits>;
	//template class BALL_EXPORT MatrixInverter<ComplexDouble, StandardTraits>;
}

#endif
