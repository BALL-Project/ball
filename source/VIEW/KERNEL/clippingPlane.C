// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: clippingPlane.C,v 1.1.2.2 2005/05/10 13:50:30 amoll Exp $
//


#include <BALL/VIEW/KERNEL/clippingPlane.h>

namespace BALL
{
	namespace VIEW
	{
		ClippingPlane::ClippingPlane()
			throw()
				: active_(false)
		{
		}

					
		ClippingPlane::ClippingPlane(const ClippingPlane& rp)
			throw()
				: normal_(rp.normal_),
					d_(rp.d_),
					active_(rp.active_)
		{
			reps_ = rp.reps_;
		}


		const ClippingPlane& ClippingPlane::operator = (const ClippingPlane& plane)
			throw()
		{
			reps_ = plane.reps_;

			normal_ = plane.normal_;
			d_ = plane.d_;
			active_ = plane.active_;
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

		
	} // namespace VIEW
} // namespace BALL
