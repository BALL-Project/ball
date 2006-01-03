#ifndef RANDOMEVALUATION_H
#define RANDOMEVALUATION_H

#include <BALL/STRUCTURE/DOCKING/conformationSet.h>
#include <BALL/DATATYPE/hashMap.h>
#include <BALL/STRUCTURE/DOCKING/energeticEvaluation.h>

#include <vector>

namespace BALL
{
		/** A basis class for energetic evaluators of docking results.
		 *	This trivial class will return the identity scoring, leaving
		 *	permutation and score unchanged.
		 */
		class RandomEvaluation : public EnergeticEvaluation
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
