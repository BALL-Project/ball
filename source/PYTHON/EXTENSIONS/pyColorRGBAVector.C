// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: pyColorRGBAVector.C,v 1.1.2.1 2005/07/25 12:37:51 amoll Exp $
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
