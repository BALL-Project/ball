// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: gridVisualisation.C,v 1.1.4.1 2007-03-25 22:02:33 oliver Exp $
//

#include <BALL/VIEW/PRIMITIVES/gridVisualisation.h>

namespace BALL
{
	namespace VIEW
	{
		GridVisualisation::GridVisualisation()
				: GeometricObject(),
				  normal_(Vector3(1.)),
					point_(),
					texture_(0),
					grid_(0)
		{
		}

					
		GridVisualisation::GridVisualisation(const GridVisualisation& rp)
				: GeometricObject(rp),
					normal_(rp.normal_),
					point_(rp.point_)
		{
		}


		const GridVisualisation& GridVisualisation::operator = (const GridVisualisation& plane)
		{
			normal_ = plane.normal_;
			point_  = plane.point_;
			return *this;
		}


	} // namespace VIEW
} // namespace BALL
