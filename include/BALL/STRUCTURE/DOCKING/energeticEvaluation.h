#ifndef ENERGETICEVALUATION_H
#define ENERGETICEVALUATION_H

#include <BALL/STRUCTURE/DOCKING/conformationSet.h>
#include <BALL/DATATYPE/hashMap.h>

#include <vector>

namespace BALL
{
		/** A basis class for energetic evaluators of docking results.
		 *	This trivial class will return the identity scoring, leaving
		 *	permutation and score unchanged.
		 */
		class EnergeticEvaluation
		{
			public:

				EnergeticEvaluation()
					throw();
				
				virtual ~EnergeticEvaluation()
					throw();
				
				/** Operations
				*/
				std::vector<ConformationSet::Conformation> operator () (const ConformationSet& conformations)
					throw();
		};
}
#endif
