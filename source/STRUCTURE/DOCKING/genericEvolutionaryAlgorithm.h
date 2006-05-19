// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: genericEvolutionaryAlgorithm.h,v 1.1.2.1 2006/05/19 14:18:23 leonhardt Exp $
// base class for all evolutionary algorithms

#ifndef _GENERIC_EVOLUTIONARY_ALGORITHM_
#define _GENERIC_EVOLUTIONARY_ALGORITHM_


namespace BALL
{
	class GenericEvolutionaryAlgorithm
	{
	public:
		//CREATE(GenericEvolutionaryAlgorithm)
		
		/** default constructor
		 */
		GenericEvolutionaryAlgorithm();
		
		// 	/** copy constructor
		// 	 */
		// 	GenericEvolutionaryAlgorithm(const GenericEvolutionaryAlgorithm& gea);
		
		/** default destructor
		 */
		virtual ~GenericEvolutionaryAlgorithm();
		
		/** assignment operator
		 */
		virtual GenericEvolutionaryAlgorithm& operator=(const GenericEvolutionaryAlgorithm& gea);
		
		/** try to optimize generic pool
		 */
		virtual void start() = 0;
		
		/** clone methode
		 */
		virtual GenericEvolutionaryAlgorithm* clone() = 0;
		
	};
}

#endif
