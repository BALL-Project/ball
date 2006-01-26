// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: RMSDMinimizer.h,v 1.1 2006/01/26 16:12:50 oliver Exp $
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
		
		void extractCAlpha(const AtomContainer& A, const AtomContainer& B);

		/**
			\return the RMSD of the optimal transformation
		*/
		double computeTransformation()
			throw(IncompatibleCoordinateSets, TooFewCoordinates);


		protected:
		std::vector<Vector3> positions_a_;
		std::vector<Vector3> positions_b_;
		Vector3 translation_1_;
		Vector3 translation_2_;
		Matrix4x4 transformation_;
	};

}	// namespace BALL

#endif // BALL_STRUCTURE_RMSDMINIMIZER_H
