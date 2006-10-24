// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:

#ifndef BALL_STRUCTURE_RANDOMEVALUATION_H
#define BALL_STRUCTURE_RANDOMEVALUATION_H

#ifndef BALL_STRUCTURE_DOCKING_RANKEDCONFORMATION_H
# include <BALL/STRUCTURE/DOCKING/conformationSet.h>
#endif

#ifndef BALL_DATATYPE_HASHMAP_H
# include <BALL/DATATYPE/hashMap.h>
#endif

#ifndef BALL_STRUCTURE_DOCKING_ENERGETICEVALUATION_H
# include <BALL/STRUCTURE/DOCKING/energeticEvaluation.h>
#endif 

#include <vector>

namespace BALL
{
		/** A basis class for energetic evaluators of docking results.
		  	This trivial class will return the identity scoring, leaving
		  	permutation and score unchanged.
				\ingroup Docking
		 */
		class BALL_EXPORT RandomEvaluation : public EnergeticEvaluation
		{
			public:

				RandomEvaluation()
					throw();
				
				virtual ~RandomEvaluation()
					throw();
				
				/** Operations
				*/
				std::vector<ConformationSet::Conformation> operator () (ConformationSet& conformations)
					throw();
		};
}
#endif
