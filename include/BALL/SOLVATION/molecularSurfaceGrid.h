// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: molecularSurfaceGrid.h,v 1.11 2003/03/03 14:18:16 anhi Exp $

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

#ifndef BALL_DATATYPE_REGULARDATA3D_H
# include <BALL/DATATYPE/regularData3D.h>
#endif

namespace BALL 
{
    /**  \addtogroup  Solvation
     *  @{
     */
	// Define the entry values for an SES or SAS grid
	const char CCONN__INSIDE = 0;
	const char CCONN__OUTSIDE = 1;
	const char CCONN__INSIDE_PROBE = (char)64;

	///
	TRegularData3D<char>* calculateSESGrid(const Vector3& lower, const Vector3& upper, 
														const float spacing, 
														const System& system, 
														const float probe_radius);

	///
	TRegularData3D<char>* calculateSASGrid(const Vector3& lower, const Vector3& upper, 
														const float spacing, 
														const System& system, 
														const float probe_radius);

   /** @} */
} // namespace BALL

#endif // BALL_SOLVATION_MOLECULARSURFACEGRID_H
