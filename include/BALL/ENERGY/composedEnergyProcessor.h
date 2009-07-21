// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: composedEnergyProcessor.h,v 1.13 2005/12/23 17:01:43 amoll Exp $
//

#ifndef BALL_ENERGY_COMPOSEDENERGYPROCESSOR
#define BALL_ENERGY_COMPOSEDENERGYPROCESSOR

#ifndef BALL_COMMON_H
# include <BALL/common.h>
#endif

#ifndef BALL_ENERGY_ENERGYPROCESSOR_H
# include <BALL/ENERGY/energyProcessor.h>
#endif

namespace BALL
{
	
	/** EnergyProcessorList
			\ingroup EnergyMiscellaneous
	*/
	typedef std::list<EnergyProcessor*> EnergyProcessorList;

	/** A class for composed energy calculations.
			Most energies are compositions of several energy contributions. To model
			this in a convenient way this class is intended to calculate and sum a
			given list of energy contributions.
			@see EnergyProcessor
    	
			\ingroup EnergyMiscellaneous
	 */	
	class BALL_EXPORT ComposedEnergyProcessor
		: public EnergyProcessor
	{

		public:

		BALL_CREATE(ComposedEnergyProcessor)

		/** @name Constructors and destructors 
		*/
		//@{

		/** Default constructor 
		*/
		ComposedEnergyProcessor();

		/** Copy constructor 
		*/
		ComposedEnergyProcessor(const ComposedEnergyProcessor& composed_energy_proc);

		/** Detailed constructor 
		*/
		ComposedEnergyProcessor(EnergyProcessorList proc_list);

		/** Destructor 
		*/
		virtual ~ComposedEnergyProcessor();

		//@}
		/** @name Assignment 
		*/
		//@{

		/** Assignment operator 
		*/
		const ComposedEnergyProcessor& operator = 
			(const ComposedEnergyProcessor& proc);

		/** Clear method.
		 * 	Clears the calculated energy and the list of processors.
		*/
		virtual void clear();
	
		//@}
		/** @name Processor functions 
		*/
		//@{
		
		/** Do all calculations and sum up the different energy contributions 
		*/
		virtual bool finish();

		//@}
		/** @name Accessors 
		*/
		//@{

		/** Add a component to the list of EnergyProcessors 
		*/
		void addComponent(EnergyProcessor* proc);

		/** remove a component from the list 
		*/
		void removeComponent(EnergyProcessor* proc);

		/** Get number of added EnergyProcessors.
		*/
		Size getNumberOfEnergyProcessors() const;
		
		//@}
		/** @name Predicates 
		*/
		//@{

		/** Equality operator 
		*/
		bool operator == (const ComposedEnergyProcessor& proc) const;

		//@}

		protected:

		EnergyProcessorList components_;


		private:

		/*_ check the validity of this instance. If one of the processors in
		 * the list is invalid, then this instance of ComposedEnergyProcessor
		 * is invalid 
		*/
		void checkValidity();

	};
} // namespace BALL
#endif //  BALL_ENERGY_COMPOSEDENERGYPROCESSOR
