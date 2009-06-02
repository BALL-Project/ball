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

#endif // BALL_LINALG_VECTOR_H
