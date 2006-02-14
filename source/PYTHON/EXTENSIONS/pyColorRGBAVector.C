// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: pyColorRGBAVector.C,v 1.2.6.1 2006/02/14 15:02:45 amoll Exp $
//

#include <BALL/PYTHON/pyColorRGBAVector.h>

namespace BALL
{

	PyColorRGBAVector::PyColorRGBAVector()
		: vector<ColorRGBA>()
	{
	}

	PyColorRGBAVector::PyColorRGBAVector(const PyColorRGBAVector& v)
		: vector<ColorRGBA>(v)
  {
	}

	PyColorRGBAVector::PyColorRGBAVector(const vector<ColorRGBA>& v)
		: vector<ColorRGBA>(v)
  {
	}

	PyColorRGBAVector::~PyColorRGBAVector()
		throw()
	{
	}

}
