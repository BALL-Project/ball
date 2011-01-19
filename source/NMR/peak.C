// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//

#include<BALL/NMR/peak.h>

namespace BALL
{

	template <>
	bool Peak<Vector3>::operator < (const Peak<Vector3>& peak) const
	{
		return ((position_.x < peak.position_.x) 
						|| ((position_.x == peak.position_.x) && 
								((position_.y < peak.position_.y) 
								 || ((position_.y == peak.position_.y) && (position_.z < peak.position_.z)))));
	}

	template <>
	bool Peak<Vector3>::operator > (const Peak<Vector3>& peak) const
	{
		return ((position_.x > peak.position_.x) 
						|| ((position_.x == peak.position_.x) && 
								((position_.y > peak.position_.y) 
								 || ((position_.y == peak.position_.y) && (position_.z > peak.position_.z)))));
	}
} // namespace BALL
