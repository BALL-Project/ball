// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: gridVisualisation.C,v 1.1.2.2 2006/05/01 20:47:10 amoll Exp $
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

		GridVolume::GridVolume()
			: GridSlice()
		{
		}


	} // namespace VIEW
} // namespace BALL
