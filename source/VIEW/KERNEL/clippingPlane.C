// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: clippingPlane.C,v 1.3.18.1 2007/03/25 22:02:21 oliver Exp $
//


#include <BALL/VIEW/KERNEL/clippingPlane.h>

namespace BALL
{
	namespace VIEW
	{
		ColorRGBA ClippingPlane::capping_color_ = ColorRGBA(0, 0, 1.0, 1.0);

		ClippingPlane::ClippingPlane()
			throw()
				: normal_(Vector3(1.)),
					point_(),
					active_(false),
					hidden_(false),
					cap_(false)
		{
		}

					
		ClippingPlane::ClippingPlane(const ClippingPlane& rp)
			throw()
				: normal_(rp.normal_),
					point_(rp.point_),
					active_(rp.active_),
					hidden_(rp.hidden_),
					cap_(rp.cap_)
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
			cap_    = plane.cap_;
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

		bool ClippingPlane::operator == (const ClippingPlane& plane) const
			throw()
		{
			return reps_   == plane.reps_   &&
				     normal_ == plane.normal_ &&
						 point_  == plane.point_  &&
						 active_ == plane.active_ &&
						 hidden_ == plane.hidden_ &&
						 cap_    == plane.cap_;
		}
		
	} // namespace VIEW
} // namespace BALL
