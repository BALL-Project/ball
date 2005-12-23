// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: molecularSurfaceGrid.h,v 1.15 2005/12/23 17:01:59 amoll Exp $
//

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

	// Define the entry values for an SES or SAS grid
	BALL_EXPORT extern const char CCONN__INSIDE;
	BALL_EXPORT extern const char CCONN__OUTSIDE;
	BALL_EXPORT extern const char CCONN__INSIDE_PROBE;

	/** calculateSESGrid
	\ingroup Solvation
	*/
	BALL_EXPORT TRegularData3D<char>* calculateSESGrid
		(const Vector3& lower, const Vector3& upper, 
		 float spacing, const System& system, float probe_radius);

	/** calculateSASGrid
	\ingroup Solvation
	*/
	BALL_EXPORT TRegularData3D<char>* calculateSASGrid
		(const Vector3& lower, const Vector3& upper, 
		 float spacing, const System& system, float probe_radius);

   
} // namespace BALL

#endif // BALL_SOLVATION_MOLECULARSURFACEGRID_H
