// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: contourSurface.C,v 1.3 2002/12/12 10:13:11 oliver Exp $

#include <BALL/DATATYPE/contourSurface.h>

namespace BALL
{

	template <>
	HashIndex Hash(const KeyType& p) 
		throw()
	{
		return (HashIndex)(p.first + p.second);
	}
  
}
