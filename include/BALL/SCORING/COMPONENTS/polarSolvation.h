// $Id: polarSolvation.h,v 1.2 2006/05/21 17:49:46 anker Exp $

#ifndef BALL_SCORING_COMPONENTS_POLARSOLVATION_H
#define BALL_SCORING_COMPONENTS_POLARSOLVATION_H

#include <BALL/SCORING/COMMON/scoringComponent.h>

#include <BALL/SOLVATION/poissonBoltzmann.h>
#include <BALL/SOLVATION/generalizedBornCase.h>

namespace BALL
{
	/** SLICK polar solvation component \\
			{\bf Definition:} \URL{BALL/SCORING/COMPONENTS/polarSolvation.h}
	*/
	class BALL_EXPORT PolarSolvation : public ScoringComponent
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
			CALCULATION__FULL_SLICK,

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


		struct BALL_EXPORT Option
		{

			/**
			*/
			static const char* POLAR_METHOD;

			/**
			*/
			static const char* POLAR_GB;

			/**
			*/
			static const char* POLAR_AVG;

			/**
			*/
			static const char* POLAR_FOCUS_GRID_AROUND_LIGAND;

			/**
			*/
			static const char* GB_SCALING_FILE;

			/**
			*/
			static const char* UNITE_ATOMS;

			/**
			*/
			static const char* VERBOSITY;

			/**
			*/
			static const char* POLAR_OVERWRITE_RADII;

			/**
			*/
			static const char* POLAR_OVERWRITE_CHARGES;

			/**
			*/
			static const char* POLAR_RADIUS_RULES;

			/**
			*/
			static const char* POLAR_CHARGE_RULES;

		};


		struct Default
		{

			/**
			*/
			static const Size POLAR_METHOD;

			/**
			*/
			static const bool POLAR_GB;

			/**
			*/
			static const Size POLAR_AVG;

			/**
			*/
			static const bool POLAR_FOCUS_GRID_AROUND_LIGAND;

			/**
			*/
			static const String GB_SCALING_FILE;

			/**
			*/
			static const bool UNITE_ATOMS;

			/**
			*/
			static const Size VERBOSITY;

			/**
			*/
			static const bool POLAR_OVERWRITE_RADII;

			/**
			*/
			static const bool POLAR_OVERWRITE_CHARGES;

			/**
			*/
			static const String POLAR_RADIUS_RULES;

			/**
			*/
			static const String POLAR_CHARGE_RULES;

		};


		/** @name	Constructors and Destructors
		*/
		//@{

		/**	Default constructor.
		*/
		PolarSolvation()
			;

		/**	Constructor.
		*/
		PolarSolvation(ScoringFunction& sf)
			;

		/**	Copy constructor
		*/
		PolarSolvation(const PolarSolvation& fhb)
			;

		/**	Destructor.
		*/
		virtual ~PolarSolvation()
			;

		//@}
		/**	@name	Assignment
		*/
		//@{

		/** Assignment.
		*/
		const PolarSolvation& operator = (const PolarSolvation& fr)
			;

		/** Clear method.
		*/
		virtual void clear()
			;

		//@}
		/**	@name	Predicates.
		*/
		//@{

		bool operator == (const PolarSolvation& fr) const
			;

		//@}
		/**	@name	Setup Methods
		*/
		//@{

		/**
		 * Setup method.
		*/
		virtual bool setup();

		void update(const vector<std::pair<Atom*, Atom*> >& pair_vector);

		/**
		 * Calculates and returns the component's energy.
		*/
		virtual double updateScore();

		//@}
		/**	@name	Accessors
		*/
		//@{

		//@}

		private:

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
		bool unite_atoms_;

		/*_
		*/
		Size verbosity_;

		/*_
		*/
		FDPB fdpb_;

		/*_
		*/
		GeneralizedBornModel gbm_;

		/*_
		*/
		bool use_gb_;

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
			;

		/*_
		*/
		bool computeESEnergy_(System& system, float& energy)
			;

		/*_
		*/
		float computeESInteractionEnergy_(const Molecule& molecule, const
				HashMap<const Atom*, float>& p_hash)
			;

		/*_
		*/
		bool computeFullCycle_(System& system, Molecule& protein,
				Molecule& ligand, float& energy)
			;

		/*_
		*/
		Vector3 permuteComponentSigns_(Vector3 vector, Size permutation)
			;

		/*_ Condense hydrogens without radius but bearing charges on the heavy
		    atom they are attached to
		*/
		Size uniteAtoms_(Molecule& molecule)
			;

	};

} // namespace BALL

#endif // BALL_SCORING_COMPONENTS_POLARSOLVATION_H
