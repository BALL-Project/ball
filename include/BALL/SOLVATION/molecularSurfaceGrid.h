// $Id: molecularSurfaceGrid.h,v 1.1 1999/08/26 07:53:20 oliver Exp $

#ifndef BALL_SOLVATION_MOLECULARSURFACEGRID_H
#define BALL_SOLVATION_MOLECULARSURFACEGRID_H

#ifndef BALL_COMMON_H
#	include <BALL/common.h>
#endif

#ifndef BALL_MATHS_VECTOR3_H
#	include <BALL/MATHS/vector3.h>
#endif

#ifndef BALL_KERNEL_SYSTEM_H
#	include <BALL/KERNEL/system.h>
#endif

#ifndef BALL_DATATYPE_POINTGRID_H
#	include <BALL/DATATYPE/pointGrid.h>
#endif

namespace BALL {

	PointGrid<char>* calculateSESGrid(const Vector3& lower, 
														const Vector3& upper, 
														const float spacing, 
														const System& system, 
														const float probe_radius);

	PointGrid<char>* calculateSASGrid(const Vector3& lower, 
														const Vector3& upper, 
														const float spacing, 
														const System& system, 
														const float probe_radius);

} // namespace BALL

#endif // BALL_SOLVATION_MOLECULARSURFACEGRID_H
