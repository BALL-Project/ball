// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: vector3.C,v 1.2 2002/02/27 12:21:29 sturm Exp $

#include <BALL/MATHS/vector3.h>

namespace BALL
{
// BALL_EXPORT on an explicit template instantiation is redundant once the
// primary template already carries visibility attributes (GCC -Wattributes).
template class TVector3<float>;

#ifdef BALL_COMPILER_MSVC
	template class BALL_EXPORT std::vector<Vector3>;
#endif

}
