// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id$
//

%TypeHeaderCode
	#include <BALL/MATHS/common.h>
	using namespace BALL::Maths;
%End

// float abs(float);
// float frac(float);
// float max(float, float);
// float max(float, float, float);
// float min(float, float);
// float min(float, float, float);
float round(float);
float sgn(float);
bool isFinite(float);
bool isInfinite(float);
bool isNan(float);
bool isZero(float);
bool isNotZero(float);
bool isEqual(float, float);
bool isNotEqual(float, float);
bool isLess(float, float);
bool isLessOrEqual(float, float);
bool isGreaterOrEqual(float, float);
bool isGreater(float, float);
long floor(float);
long ceiling(float);
Index compare(float, float);
bool isNear(float, float, float);
