// $Id: fresnoNonPolar.h,v 1.1.2.3 2002/11/19 01:48:57 anker Exp $

#ifndef BALL_MOLMEC_FRESNO_FRESNONONPOLAR_H
#define BALL_MOLMEC_FRESNO_FRESNONONPOLAR_H

#include <BALL/MOLMEC/COMMON/forceFieldComponent.h>
#include <BALL/SOLVATION/uhligCavFreeEnergyProcessor.h>
#include <BALL/SOLVATION/PCMCavFreeEnergyProcessor.h>
#include <BALL/SOLVATION/pair6_12InteractionEnergyProcessor.h>

namespace BALL
{

	/** Fresno Solvation component.
			{\bf Definition:} \URL{BALL/MOLMEC/FRESNO/fresnoNonPolar.h}
	*/
	class FresnoNonPolar
		:	public ForceFieldComponent
	{

		public:

		/**
		*/
		enum CalculationMethod
		{

			/**
			*/
			CALCULATION__NONE,

			/**
			*/
			CALCULATION__UHLIG,

			/**
			*/
			CALCULATION__PCM,

			/** 
			*/
			CALCULATION__VDW_SOLVENT,

			/** 
			*/
			CALCULATION__VDW_INTERACTION

		};

		/** @name	Constructors and Destructors	
		*/
		//@{ 

		/**	Default constructor.
		*/
		FresnoNonPolar()
			throw();

		/**	Constructor.
		*/
		FresnoNonPolar(ForceField& force_field)
			throw();

		/**	Copy constructor
		*/
		FresnoNonPolar(const FresnoNonPolar& fhb, bool clone_deep = true)
			throw();

		/**	Destructor.
		*/
		virtual ~FresnoNonPolar()
			throw();

		//@}
		/**	@name	Assignment
		*/
		//@{

		/** Assignment.
		*/
		const FresnoNonPolar& operator = (const FresnoNonPolar& fr)
			throw();

		/** Clear method.
		*/
		virtual void clear()
			throw();

		//@}
		/**	@name	Predicates.
		*/
		//@{

		bool operator == (const FresnoNonPolar& fr) const
			throw();

		//@}
		/**	@name	Setup Methods	
		*/
		//@{

		/**	Setup method.
		*/
		virtual bool setup()
			throw();

		//@}
		/**	@name	Accessors	
		*/
		//@{

		/**	Calculates and returns the component's energy.
		*/
		virtual double updateEnergy()
			throw();

		/**	Calculates and returns the component's forces.
		*/
		virtual void updateForces()
			throw();

		//@}

		private:

		/*_
		*/
		Size verbosity_;

		/*_
		*/
		double factor_;

		/*_
		*/
		Size calculation_method_;

		/*_
		*/
		float probe_radius_;

		/*_
		*/
		float surface_tension_;

		/*_
		*/
		float constant_;

		/*_
		*/
		float solvent_number_density_;

		/*_
		*/
		float absolute_temperature_;

		/*_
		*/
		UhligCavFreeEnergyProcessor uhlig_;

		/*_
		*/
		PCMCavFreeEnergyProcessor pcm_;

		/*_
		*/
		Pair6_12InteractionEnergyProcessor vdw_solvent_;

	};

} // namespace BALL

#endif // BALL_MOLMEC_FRESNO_FRESNONONPOLAR_H

