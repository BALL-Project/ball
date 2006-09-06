// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: spectrum.C,v 1.2.20.1 2006/09/06 12:23:24 anne Exp $

#include <BALL/NMR/spectrum.h>
#include <BALL/DATATYPE/regularData1D.h>
#include <BALL/DATATYPE/regularData2D.h>
#include <BALL/DATATYPE/regularData3D.h>

# ifdef BALL_NO_INLINE_FUNCTIONS
#   include <BALL/NMR/spectrum.iC>
# endif

using namespace BALL;

std::ostream& operator << (std::ostream& os, const Spectrum<RegularData2D, Peak2D>& spectrum)
{
	os << spectrum.getData();

	return os;
}
