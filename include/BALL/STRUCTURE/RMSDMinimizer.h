// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: RMSDMinimizer.h,v 1.2 2006/01/27 20:57:46 oliver Exp $
//
// Author:
//   Oliver Kohlbacher
//

#ifndef BALL_STRUCTURE_RMSDMINIMIZER_H
#define BALL_STRUCTURE_RMSDMINIMIZER_H

#ifndef BALL_STRUCTURE_ATOMBIJECTION_H
#	include <BALL/STRUCTURE/atomBijection.h>
#endif

#ifndef BALL_MATHS_MATRIX44_H
#	include <BALL/MATHS/matrix44.h>
#endif

namespace BALL
{

	/** RMSD minimizer class.
			
			 \par
	\ingroup StructureMiscellaneous
	\remarks{
		This class requires GSL (GNU Scientific Library) support to be enabled.
		If you cannot compile/link code using RMSDMinimizer, please rerun configure
		and enable GSL support (configure --help for details).
	}
	*/
	class BALL_EXPORT RMSDMinimizer
	{		
		public:
		class BALL_EXPORT IncompatibleCoordinateSets
			:	public Exception::GeneralException
		{
			public:
			IncompatibleCoordinateSets(const char*, int, Size, Size);
		};
		
		class BALL_EXPORT TooFewCoordinates
			:	public Exception::GeneralException
		{
			public:
			TooFewCoordinates(const char*, int, Size);
		};

		typedef std::vector<Vector3> PointVector;
		typedef std::pair<Matrix4x4, double> Result;

		static Result computeTransformation(const AtomBijection& ab)
			throw(RMSDMinimizer::IncompatibleCoordinateSets, RMSDMinimizer::TooFewCoordinates);

		static Result computeTransformation(const PointVector& X, const PointVector& Y)
			throw(RMSDMinimizer::IncompatibleCoordinateSets, RMSDMinimizer::TooFewCoordinates);
 };

}	// namespace BALL

#endif // BALL_STRUCTURE_RMSDMINIMIZER_H
