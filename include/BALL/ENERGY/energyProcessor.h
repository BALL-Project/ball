// $Id: energyProcessor.h,v 1.6 2001/02/27 01:58:22 amoll Exp $

#ifndef BALL_ENERGY_ENERGYPROCESSOR_H
#define BALL_ENERGY_ENERGYPROCESSOR_H

#ifndef BALL_COMMON_H
#include <BALL/common.h>
#endif

#ifndef BALL_KERNEL_ATOMCONTAINER_H
#include <BALL/KERNEL/atomContainer.h>
#endif

#ifndef BALL_CONCEPT_PROCESSOR_H
#include <BALL/CONCEPT/processor.h>
#endif

namespace BALL
{
	/** Processor for energy calculations.
			With this class we provide an interface to different (free) energy
			calculations.
			{\bf Definition:} \URL{BALL/ENERGY/energyProcessor.h}
	*/
	class EnergyProcessor
		:	public UnaryProcessor<AtomContainer>
	{

		public:

		BALL_CREATE(EnergyProcessor)

		/** @name Constructors and Destructors 
		*/
		//@{

		/** Default constructor 
		*/
		EnergyProcessor() throw();

		/** Copy constructor 
		*/
		EnergyProcessor(const EnergyProcessor& proc) throw();

		/** Destructor 
		*/
		virtual ~EnergyProcessor() throw();

		//@}
		/** @name Assignment 
		*/
		//@{

		/** */
		const EnergyProcessor& operator = (const EnergyProcessor& proc) throw();

		/** */
		virtual void clear() throw();

		//@}
		/** @name	Processor related methods
		*/
		//@{

		/** Start-function. 
		*/
		virtual bool start() throw();

		/** Operator-function. This function only stores the AtomContainer. 
		*/
		virtual Processor::Result operator () (AtomContainer& fragment) throw();
		
		//@}
		/** @name Accessors 
		*/
		//@{

		/** Return the computed energy 
		*/
		double getEnergy() const throw();

		//@}	
		/** @name Predicates 
		*/
		//@{

		/** Validity 
		*/
		bool isValid() const throw();

		/** Equality operator 
		*/
		bool operator == (const EnergyProcessor& proc) const throw();

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
