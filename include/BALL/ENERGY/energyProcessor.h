// $Id: energyProcessor.h,v 1.1 2000/08/31 14:16:56 anker Exp $

#ifndef BALL_ENERGY_ENERGYPROCESSOR_H
#define BALL_ENERGY_ENERGYPROCESSOR_H

#ifndef BALL_COMMON_H
#include <BALL/common.h>
#endif

#ifndef BALL_KERNEL_BASEFRAGMENT_H
#include <BALL/KERNEL/baseFragment.h>
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
		:	public UnaryProcessor<BaseFragment>
	{

		public:

		// BALL_CREATE_NODEEP(EnergyProcessor)

		/** @name Constructors and Destructors */
		//@{

		/** Default constructor */
		EnergyProcessor();

		/** */
		EnergyProcessor(const EnergyProcessor& proc);

		/** Destructor */
		virtual ~EnergyProcessor();

		/** */
		virtual void destroy();

		/** */
		virtual void clear();
		//@}


		/** @name Assignment */
		//@{

		/** */
		void set(const EnergyProcessor& proc);

		/** */
		const EnergyProcessor& operator = (const EnergyProcessor& proc);

		//@}


		/** @name Hmmm. */
		//@{

		/** Start-function. */
		virtual bool start();

		/** Operator-function. This function only stores the BaseFragment. */
		virtual Processor::Result operator () (BaseFragment& fragment);
		
		//@}


		/** @name Accessors */
		//@{

		/** return the computed energy */
		double getEnergy() const;

		//@}

		protected:

		const BaseFragment* fragment_;
		double energy_;
	};
}

#endif // BALL_ENERGY_ENERGYPROCESSOR_H
