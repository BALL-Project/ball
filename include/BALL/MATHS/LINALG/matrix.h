#ifndef BALL_LINALG_MATRIX_H
#define BALL_LINALG_MATRIX_H

namespace BALL {

	class StandardTraits;

  template <class valuetype>
  class Vector;

  template <class valuetype, class mtraits=StandardTraits>
  class Matrix;
}

#include <BALL/MATHS/LINALG/matrix.ih>
#include <BALL/MATHS/LINALG/vector.ih>

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
	

#include <BALL/MATHS/LINALG/vectorConstructors.iC>
#include <BALL/MATHS/LINALG/vectorIO.iC>
#include <BALL/MATHS/LINALG/vectorBasicFunctions.iC>

#endif // MALL_KERNEL_MATRIX_H
