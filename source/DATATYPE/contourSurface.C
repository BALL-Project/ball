// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: contourSurface.C,v 1.2.2.1 2002/10/29 15:56:36 anhi Exp $

#include <BALL/DATATYPE/contourSurface.h>

namespace BALL
{

	template<>
	HashIndex BALL::Hash(const KeyType& p)
	{
		return (HashIndex)(p.first + p.second);
	}
  
}
