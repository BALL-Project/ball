// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: contourSurface.C,v 1.3.2.1 2003/01/07 13:20:35 anker Exp $

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
