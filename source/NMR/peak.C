// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: peak.C,v 1.10 2002/02/27 12:23:54 sturm Exp $

#include<BALL/NMR/peak.h>

namespace BALL
{

	template <>
	bool Peak<Vector3>::operator < (const Peak<Vector3>& peak) const
	{
		return ((position_.x < peak.position_.x) 
						|| ((position_.x == peak.position_.x) && 
								((position_.y < peak.position_.y) 
								 || (position_.y == peak.position_.y) && (position_.z < peak.position_.z))));
	}

	template <>
	bool Peak<Vector3>::operator > (const Peak<Vector3>& peak) const
	{
		return ((position_.x > peak.position_.x) 
						|| ((position_.x == peak.position_.x) && 
								((position_.y > peak.position_.y) 
								 || (position_.y == peak.position_.y) && (position_.z > peak.position_.z))));
	}
} // namespace BALL
