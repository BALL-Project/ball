// $Id: fresnoDesolvation.h,v 1.1.2.8 2004/04/28 15:37:49 anker Exp $

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
			CALCULATION__NONE,

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
			CALCULATION__FULL_CYCLE_FOCUSED,

			/**
			*/
			CALCULATION__COULOMB,

			/**
			*/
			CALCULATION__EEF1

		};

		/** Averaging methods for model 0 and 3
		*/
		enum AveragingMethod
		{
			/** No averaging will be done.
			*/
			AVERAGING__NONE,

			/** The grid of the PB solver will be translated a bit in every
					direction by a fixed length.
			*/
			AVERAGING__STATIC,

			/** The grid of the PB solver will be translated a bit in every
					direction by a randomly chosen length (not yet implemented).
			*/
			AVERAGING__RANDOM_FACTOR,

			/** The grid of the PB solver will be translated a bit in every
					direction by different randomly chosen length (not yet
					implemented).
			*/
			AVERAGING__RANDOM

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
		FresnoDesolvation(const FresnoDesolvation& fhb)
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
		Molecule desolv_protein_;

		/*_
		*/
		Molecule desolv_ligand_;

		/*_
		*/
		Size calculation_method_;

		/*_
		*/
		Size averaging_;

		/*_
		*/
		Size verbosity_;

		/*_
		*/
		FDPB fdpb_;

		/*_
		*/
		float spacing_;

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
		float computeESInteractionEnergy_(const Molecule& molecule)
			throw();

		/*_
		*/
		bool computeFullCycle_(System& system, Molecule& protein, 
				Molecule& ligand, float& energy)
			throw();

		/*_
		*/
		Vector3 permuteComponentSigns_(Vector3 vector, Size permutation)
			throw();


	};

} // namespace BALL

#endif // BALL_MOLMEC_FRESNO_FRESNODESOLVATION_H

