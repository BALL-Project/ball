// $Id: peak.C,v 1.9 2001/06/14 11:34:59 oliver Exp $

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
