// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// Phase 999.60 (v1.7.x-25) — Owner-narrowing NEGATIVE compile test (Rep).
//
// ARCHITECTURE-CONTRACT.md §3c / §11c. This TU mutates a Representation
// owner DIRECTLY from a non-Controller / non-builder translation unit.
// Per §3c the only legal mutation surface is the RepresentationBuilder
// friend; direct setX() from outside must become a COMPILE ERROR once
// 999.59-04 privatises the setters behind `friend class
// RepresentationBuilder`.
//
// Today the setters are still PUBLIC, so this file COMPILES; the driver
// records the expected outcome accordingly and flips it after 999.59-04.
//

#include <BALL/VIEW/KERNEL/representation.h>
#include <BALL/VIEW/KERNEL/common.h>

using namespace BALL;
using namespace BALL::VIEW;

void bypass_representation_mutation_from_outside()
{
	Representation rep;
	// §3c: these must NOT compile from a non-builder TU after 999.59-04.
	rep.setModelType(MODEL_VDW);
	rep.setColoringMethod(COLORING_ELEMENT);
	rep.setTransparency(64);
	rep.setDrawingMode(DRAWING_MODE_SOLID);
}
