// $Id: fresnoDesolvation.h,v 1.1.2.3 2002/09/13 14:07:00 anker Exp $
// Molecular Mechanics: Fresno force field, desolvation component

#ifndef BALL_MOLMEC_FRESNO_FRESNODESOLVATION_H
#define BALL_MOLMEC_FRESNO_FRESNODESOLVATION_H

#include <BALL/SOLVATION/poissonBoltzmann.h>
#include <BALL/MOLMEC/COMMON/forceFieldComponent.h>

namespace BALL
{

	/** Fresno Hydrogen Bond component.
			{\bf Definition:} \URL{BALL/MOLMEC/FRESNO/fresnoDesolvation.h}
	*/
	class FresnoDesolvation
		:	public ForceFieldComponent
	{

		public:

		/**
		*/
		enum CalculationMethod
		{
			/**
			*/
			CALCULATION__FRESNO,

			/**
			*/
			CALCULATION__FULL_FRESNO,

			/**
			*/
			CALCULATION__FULL_CYCLE,

			/**
			*/
			CALCULATION__KEKSE,

			/**
			*/
			CALCULATION__EEF1

		};

		/** @name	Constructors and Destructors	
		*/
		//@{ 

		/**	Default constructor.
		*/
		FresnoDesolvation()
			throw();

		/**	Constructor.
		*/
		FresnoDesolvation(ForceField& force_field)
			throw();

		/**	Copy constructor
		*/
		FresnoDesolvation(const FresnoDesolvation& fhb, bool clone_deep = true)
			throw();

		/**	Destructor.
		*/
		virtual ~FresnoDesolvation()
			throw();

		//@}
		/**	@name	Assignment
		*/
		//@{

		/** Assignment.
		*/
		const FresnoDesolvation& operator = (const FresnoDesolvation& fr)
			throw();

		/** Clear method.
		*/
		virtual void clear()
			throw();

		//@}
		/**	@name	Predicates.
		*/
		//@{

		bool operator == (const FresnoDesolvation& fr) const
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
		double factor_;

		/*_
		*/
		Size calculation_method_;

		/*_
		*/
		Size verbosity_;

		/*_
		*/
		FDPB fdpb_;

		/*_
		*/
		float bulk_water_dc_;

		/*_
		*/
		float vacuum_dc_;

		/*_
		*/
		bool computeEnergyDifference_(System& system, float& energy)
			throw();

		/*_
		*/
		bool computeESEnergy_(System& system, float& energy)
			throw();

		/*_
		*/
		bool computeFullCycle_(System& system, Molecule& protein, 
				Molecule& ligand, float& energy)
			throw();

	};

} // namespace BALL

#endif // BALL_MOLMEC_FRESNO_FRESNODESOLVATION_H
