// $Id: composedEnergyProcessor.h,v 1.1 2000/08/31 18:45:30 anker Exp $

// BAUSTELLE: Pfad?
#ifndef BALL_ENERGY_COMPOSEDENERGYPROCESSOR
#define BALL_ENERGY_COMPOSEDENERGYPROCESSOR

#ifndef BALL_COMMON_H
#include <BALL/common.h>
#endif

#ifndef BALL_DATATYPE_LIST_H
#include <BALL/DATATYPE/list.h>
#endif

// BAUSTELLE: Pfad?
#ifndef BALL_ENERGY_ENERGYPROCESSOR
#include "energyProcessor.h"
#endif

namespace BALL
{
	typedef List<EnergyProcessor*> EnergyProcessorList;

	/** A class for composed energy calculations.
		Most energies are compositions of several energy contributions. To model
		this in a convenient way this class is intended to calculate and sum a
		given list of energy contributions (@see EnergyProcessor).
	 */
	class ComposedEnergyProcessor
		:	public EnergyProcessor
	{

		public:

		// BALL_CREATE(ComposedEnergyProcessor)

		/** @name Constructors and destructors */
		//@{

		/** Default constructor */
		ComposedEnergyProcessor();

		/** Copy constructor */
		ComposedEnergyProcessor(const ComposedEnergyProcessor& composed_energy_proc);

		/** Detailed constructor */
		ComposedEnergyProcessor(EnergyProcessorList proc_list);

		/** Destructor */
		virtual ~ComposedEnergyProcessor();

		/** */
		virtual void destroy();

		/** */
		virtual void clear();

		//@}


		/** @name Assignment */
		//@{

		/** */
		void set(const ComposedEnergyProcessor& proc);

		/** */
		const ComposedEnergyProcessor& operator = 
			(const ComposedEnergyProcessor& proc);

		//@}


		/** @name Processor functions */
		//@{
		
		/** Do all calculations and sum up the different energy contributions */
		virtual bool finish();

		//@}


		/** @name Accessors */
		//@{

		/** Add a component to the list of EnergyProcessors */
		void addComponent(EnergyProcessor* proc);

		/** remove a component from the list */
		void removeComponent(EnergyProcessor* proc);

		/** return the energy value */
		// double getEnergy();

		//@}

		protected:

		EnergyProcessorList components_;

	};

} // namespace BALL
#endif //  BALL_ENERGY_COMPOSEDENERGYPROCESSOR
