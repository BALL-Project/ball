// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:

#ifndef BALL_SCORING_FUNCTIONS_ENERGETICEVALUATION_H
#define BALL_SCORING_FUNCTIONS_ENERGETICEVALUATION_H

#ifndef BALL_DOCKING_COMMON_CONFORMATIONSET_H
# include <BALL/DOCKING/COMMON/conformationSet.h>
#endif

#include <vector>

namespace BALL
{
	/** Base class for energetic evaluators of docking results.
		This trivial class will return the identity scoring, leaving
		permutation and score unchanged.
			\ingroup Docking
	 */
	class BALL_EXPORT EnergeticEvaluation
	{
		public:
			EnergeticEvaluation()
				throw();

			virtual ~EnergeticEvaluation()
				throw();

			/** Operations
			*/
			virtual std::vector < ConformationSet::Conformation > operator () (ConformationSet& conformations);
	};
}

#endif // BALL_SCORING_FUNCTIONS_ENERGETICEVALUATION_H
