#include <BALL/DATATYPE/hashMap.h>
#include "rankedConformations.h"

#ifndef ENERGETICEVALUATION_H
#define ENERGETICEVALUATION_H

namespace BALL
{

	namespace Docking
	{

		class EnergyGrid;

		/** A class defining the interface for docking algorithms.

				This is a first iteration.

		*/
		class EnergeticEvaluation
		{

			public:

				/** Attributes
				*/
		                
		                

				/** Operations
				*/
		               
				void operator () (RankedConformations& ranked_conformations) const 
				      throw();
						


		};

	}

}
#endif
