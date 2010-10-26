#ifndef BALL_LINALG_VECTOR_H
#define BALL_LINALG_VECTOR_H

#ifndef BALL_COMMON_GLOBAL_H
# include <BALL/COMMON/global.h>
#endif

namespace BALL {

  template <class valuetype>
  class Vector;

  template <class valuetype, class mtraits>
  class Matrix;
}

#include <BALL/MATHS/LINALG/vector.ih>

namespace BALL
{
	// explicit instantiation needed to export the template
	template class BALL_EXPORT Vector<float>;
	template class BALL_EXPORT Vector<double>;
	template class BALL_EXPORT Vector<ComplexFloat>;
	template class BALL_EXPORT Vector<ComplexDouble>;
}

#include <BALL/MATHS/LINALG/matrix.h>

#include <BALL/MATHS/LINALG/vectorConstructors.iC>
#include <BALL/MATHS/LINALG/vectorIO.iC>
#include <BALL/MATHS/LINALG/vectorBasicFunctions.iC>

#endif // BALL_LINALG_VECTOR_H
