// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: contourSurface.C,v 1.2 2002/09/06 10:44:56 aubertin Exp $

#include <BALL/DATATYPE/contourSurface.h>

namespace BALL
{

	template<>
	HashIndex BALL::Hash(const KeyType& p)
	{
		return (HashIndex)(p.first + p.second);
	}
  
}
