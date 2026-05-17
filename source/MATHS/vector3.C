// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: vector3.C,v 1.2 2002/02/27 12:21:29 sturm Exp $

#include <BALL/MATHS/vector3.h>

namespace BALL
{
// BALL_EXPORT is REQUIRED on this explicit template instantiation for MSVC
// DLL export — without it BALL.dll does not export TVector3<float>::* symbols
// and downstream targets (VIEW.dll, BALLView, tests) fail to link with 35+
// "unresolved external symbol" errors. The earlier "redundant on GCC
// -Wattributes" attempt to drop BALL_EXPORT (commit cb392cc4) caused exactly
// that link breakage on release.yml's Windows cold-cache job — ci.yml masked
// the defect because the Phase 999.17 cmake-tree cache key
// (hashFiles('CMakeLists.txt','cmake/**','vcpkg.json','CMakePresets.json'))
// does not include source files, so the cached .lib import library was
// reused stale. For the GCC -Wattributes warning use `#pragma GCC diagnostic`
// scope, NOT removal of the export contract. Cross-ref: Phase 5.1 Plan 07
// canonical MSVC DLL-export pattern.
#if defined(BALL_COMPILER_GXX)
#  pragma GCC diagnostic push
#  pragma GCC diagnostic ignored "-Wattributes"
#endif
template class BALL_EXPORT TVector3<float>;
#if defined(BALL_COMPILER_GXX)
#  pragma GCC diagnostic pop
#endif

#ifdef BALL_COMPILER_MSVC
	template class BALL_EXPORT std::vector<Vector3>;
#endif

}
