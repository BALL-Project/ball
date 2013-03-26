// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
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
			This class computes the optimal transformation mapping
			one set of three-dimensional points onto another set of
			points. It implements the algorithm by Coutsalis et al.
			(J. Comput. Chem., 25(15), 1849 (2004)), which computes
			the RMSD-optimal transformation by solving an eigenvalue
			problem.
			 \par
	\ingroup StructureMapping
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

		static double minimizeRMSD(AtomContainer& a, AtomContainer& b)
			throw(RMSDMinimizer::IncompatibleCoordinateSets, RMSDMinimizer::TooFewCoordinates);

 };

}	// namespace BALL

#endif // BALL_STRUCTURE_RMSDMINIMIZER_H
