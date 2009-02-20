// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: energyProcessor.h,v 1.17 2005/12/23 17:01:43 amoll Exp $
//

#ifndef BALL_ENERGY_ENERGYPROCESSOR_H
#define BALL_ENERGY_ENERGYPROCESSOR_H

#ifndef BALL_COMMON_H
# include <BALL/common.h>
#endif

#ifndef BALL_KERNEL_ATOMCONTAINER_H
# include <BALL/KERNEL/atomContainer.h>
#endif

#ifndef BALL_CONCEPT_PROCESSOR_H
# include <BALL/CONCEPT/processor.h>
#endif

namespace BALL
{
	/** Processor for energy calculations.
			With this class we provide an interface to different (free) energy
			calculations.
			
	 		\ingroup EnergyMiscellaneous
	*/
	class BALL_EXPORT EnergyProcessor
		:	public UnaryProcessor<AtomContainer>
	{

		public:

		BALL_CREATE(EnergyProcessor)

		/** @name Constructors and Destructors 
		*/
		//@{

		/** Default constructor 
		*/
		EnergyProcessor();

		/** Copy constructor 
		*/
		EnergyProcessor(const EnergyProcessor& proc);

		/** Destructor 
		*/
		virtual ~EnergyProcessor();

		//@}
		/** @name Assignment 
		*/
		//@{

		/** Assignment operator.
		 */ 
		const EnergyProcessor& operator = (const EnergyProcessor& proc);

		/** Clear method.
		 */
		virtual void clear();

		//@}
		/** @name	Processor related methods
		*/
		//@{

		/** Start-function. 
		*/
		virtual bool start();

		/** Operator-function. This function only stores the AtomContainer. 
		*/
		virtual Processor::Result operator () (AtomContainer& fragment);
		
		//@}
		/** @name Accessors 
		*/
		//@{

		/** Return the computed energy 
		*/
		double getEnergy() const;

		//@}	
		/** @name Predicates 
		*/
		//@{

		/** Validity.
		*/
		bool isValid() const;

		/** Equality operator.
		*/
		bool operator == (const EnergyProcessor& proc) const;

		//@}

		protected:

		/*_ this is the Fragment for which the computation is to be done */
		const AtomContainer* fragment_;

		/*_ the calculated energy */
		double energy_;

		/*_ the valid flag */
		bool valid_;

	};
}

#endif // BALL_ENERGY_ENERGYPROCESSOR_H
