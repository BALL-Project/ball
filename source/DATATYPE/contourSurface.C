// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: contourSurface.C,v 1.2.2.2 2002/11/26 13:59:13 oliver Exp $

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
