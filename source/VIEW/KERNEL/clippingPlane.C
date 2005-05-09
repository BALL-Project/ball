// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: clippingPlane.C,v 1.1.2.1 2005/05/09 15:37:09 amoll Exp $
//


#include <BALL/VIEW/KERNEL/clippingPlane.h>

namespace BALL
{
	namespace VIEW
	{
		ClippingPlane::ClippingPlane()
			throw()
				: Representation()
		{
		}

					
		ClippingPlane::ClippingPlane(const ClippingPlane& rp)
			throw()
				: Representation(rp),
					x_(rp.x_),
					y_(rp.y_),
					z_(rp.z_),
					d_(rp.d_)
		{
			reps_ = rp.reps_;
		}


		const ClippingPlane& ClippingPlane::operator = (const ClippingPlane& plane)
			throw()
		{
			Representation::operator = (plane);
			reps_ = plane.reps_;

			x_ = plane.x_;
			y_ = plane.y_;
			z_ = plane.z_;
			d_ = plane.d_;
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
