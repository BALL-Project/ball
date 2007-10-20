// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:

#ifndef BALL_STRUCTURE_DOCKING_ENERGETICEVALUATION_H
#define BALL_STRUCTURE_DOCKING_ENERGETICEVALUATION_H

#ifndef BALL_STRUCTURE_DOCKING_RANKEDCONFORMATION_H
# include <BALL/STRUCTURE/DOCKING/conformationSet.h>
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
				virtual std::vector<ConformationSet::Conformation> operator () (ConformationSet& conformations);
		};
}
#endif
