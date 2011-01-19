// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//

#include <limits>
#include <BALL/VIEW/DATATYPE/raytraceableGrid.h>
namespace BALL
{
	namespace VIEW
	{
		void RaytraceableGrid::binaryWrite(const String& filename) const 
		{	
			grid_->binaryWriteRaw(filename);						
		}

		float RaytraceableGrid::getGridMinValue()
		{
			float current_min = std::numeric_limits<float>::max(); 
			if (grid_ != 0)
			{
				RegularData3D::Iterator rd3d_it;
				current_min = grid_->getData(0);
				for (rd3d_it = grid_->begin(); rd3d_it != grid_->end(); rd3d_it++)
				{
					if (*rd3d_it < current_min)
					{
						current_min = *rd3d_it;
					}
				}
			}
			return current_min;
		}
		
		float RaytraceableGrid::getGridMaxValue()
		{
			float current_max = std::numeric_limits<float>::min(); 
			if (grid_ != 0)
			{
				RegularData3D::Iterator rd3d_it;
				current_max = grid_->getData(0);
				for (rd3d_it = grid_->begin(); rd3d_it != grid_->end(); rd3d_it++)
				{
					if (*rd3d_it > current_max)
					{
						current_max = *rd3d_it;
					}
				}
			}
			return current_max;
		}
	}

}
