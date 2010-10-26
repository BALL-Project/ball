#ifndef BALL_LINALG_MATRIX_H
#define BALL_LINALG_MATRIX_H

#ifndef BALL_COMMON_GLOBAL_H
# include <BALL/COMMON/global.h>
#endif

namespace BALL {

	class StandardTraits;

  template <class valuetype>
  class Vector;

  template <class valuetype, class mtraits=StandardTraits>
  class Matrix;
}

#include <BALL/MATHS/LINALG/matrix.ih>

namespace BALL
{
	// explicit instantiation needed to export the template
	template class BALL_EXPORT Matrix<float, StandardTraits>;
	template class BALL_EXPORT Matrix<double, StandardTraits>;
	template class BALL_EXPORT Matrix<ComplexFloat, StandardTraits>;
	template class BALL_EXPORT Matrix<ComplexDouble, StandardTraits>;
}

#include <BALL/MATHS/LINALG/vector.h>

#include <BALL/MATHS/LINALG/upperTriangularMatrix.ih>
#include <BALL/MATHS/LINALG/upperTriangularMatrixConstructors.iC>
#include <BALL/MATHS/LINALG/upperTriangularMatrixBasicFunctions.iC>
#include <BALL/MATHS/LINALG/upperTriangularMatrixIO.iC>

#include <BALL/MATHS/LINALG/lowerTriangularMatrix.ih>
#include <BALL/MATHS/LINALG/lowerTriangularMatrixConstructors.iC>
#include <BALL/MATHS/LINALG/lowerTriangularMatrixBasicFunctions.iC>
#include <BALL/MATHS/LINALG/lowerTriangularMatrixIO.iC>

#include <BALL/MATHS/LINALG/symmetricMatrix.ih>
#include <BALL/MATHS/LINALG/symmetricMatrixConstructors.iC>
#include <BALL/MATHS/LINALG/symmetricMatrixBasicFunctions.iC>
#include <BALL/MATHS/LINALG/symmetricMatrixIO.iC>

#include <BALL/MATHS/LINALG/matrixConstructors.iC>
#include <BALL/MATHS/LINALG/matrixBasicFunctions.iC>
#include <BALL/MATHS/LINALG/matrixIO.iC>

#endif // MALL_KERNEL_MATRIX_H
