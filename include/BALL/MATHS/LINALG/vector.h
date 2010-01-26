#ifndef BALL_LINALG_VECTOR_H
#define BALL_LINALG_VECTOR_H

namespace BALL {

  template <class valuetype>
  class Vector;

  template <class valuetype, class mtraits>
  class Matrix;
}

#include <BALL/MATHS/LINALG/vector.ih>
#include <BALL/MATHS/LINALG/matrix.ih>	

#include <BALL/MATHS/LINALG/vectorConstructors.iC>
#include <BALL/MATHS/LINALG/vectorIO.iC>
#include <BALL/MATHS/LINALG/vectorBasicFunctions.iC>

#include <BALL/MATHS/LINALG/matrixConstructors.iC>
#include <BALL/MATHS/LINALG/matrixBasicFunctions.iC>
#include <BALL/MATHS/LINALG/matrixIO.iC>

namespace BALL {
	// explicit instantiation to save the compiler some work (and to *make* it work on Windows)
	template class BALL_EXPORT Vector<float>;
	template class BALL_EXPORT Vector<double>;
	template class BALL_EXPORT Vector<ComplexFloat>;
	template class BALL_EXPORT Vector<ComplexDouble>;
}

#endif // BALL_LINALG_VECTOR_H
