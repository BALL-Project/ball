// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: vector3.C,v 1.2 2002/02/27 12:21:29 sturm Exp $

#include <BALL/MATHS/vector3.h>

namespace BALL
{
template class BALL_EXPORT TVector3<float>;

#ifdef BALL_COMPILER_MSVC
	template class BALL_EXPORT std::vector<Vector3>;
#endif

}
