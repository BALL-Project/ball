// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: gridVisualisation.C,v 1.1.2.1 2006/04/27 12:17:22 amoll Exp $
//

#include <BALL/VIEW/PRIMITIVES/gridVisualisation.h>

namespace BALL
{
	namespace VIEW
	{
		GridSlice::GridSlice()
			throw()
				: GeometricObject(),
				  normal_(Vector3(1.)),
					point_(),
					double_(false),
					texture_(0),
					grid_(0)
		{
		}

					
		GridSlice::GridSlice(const GridSlice& rp)
			throw()
				: GeometricObject(rp),
					normal_(rp.normal_),
					point_(rp.point_)
		{
		}


		const GridSlice& GridSlice::operator = (const GridSlice& plane)
			throw()
		{
			normal_ = plane.normal_;
			point_  = plane.point_;
			return *this;
		}


	} // namespace VIEW
} // namespace BALL
