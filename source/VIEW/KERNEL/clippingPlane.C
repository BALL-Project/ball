// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: clippingPlane.C,v 1.1.2.4 2005/06/15 13:36:58 amoll Exp $
//


#include <BALL/VIEW/KERNEL/clippingPlane.h>

namespace BALL
{
	namespace VIEW
	{
		ClippingPlane::ClippingPlane()
			throw()
				: normal_(Vector3(1.)),
					point_(),
					active_(false),
					hidden_(false)
		{
		}

					
		ClippingPlane::ClippingPlane(const ClippingPlane& rp)
			throw()
				: normal_(rp.normal_),
					point_(rp.point_),
					active_(rp.active_),
					hidden_(rp.hidden_)
		{
			reps_ = rp.reps_;
		}


		const ClippingPlane& ClippingPlane::operator = (const ClippingPlane& plane)
			throw()
		{
			reps_   = plane.reps_;
			normal_ = plane.normal_;
			active_ = plane.active_;
			point_  = plane.point_;
			hidden_ = plane.hidden_;
			return *this;
		}


		ClippingPlane::~ClippingPlane()
			throw()
		{
			clear();
		}

		void ClippingPlane::clear()
			throw()
		{
			reps_.clear();
		}

		float ClippingPlane::getDistance() const
		{
			return normal_ * (- point_);
		}

		
	} // namespace VIEW
} // namespace BALL
