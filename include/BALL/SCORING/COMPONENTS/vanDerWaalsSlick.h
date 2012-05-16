// $Id: vanDerWaals.h,v 1.3 2006/05/21 17:35:26 anker Exp $
// Molecular Mechanics: SLICK force field, modified van-der-Waals term

#ifndef BALL_SCORING_COMPONENTS_VANDERWAALSSLICK_H
#define BALL_SCORING_COMPONENTS_VANDERWAALSSLICK_H

#include <BALL/SCORING/COMMON/scoringComponent.h>
#include <BALL/MOLMEC/AMBER/amberNonBonded.h>
#include <BALL/MOLMEC/PARAMETER/lennardJones.h>
#include <BALL/MOLMEC/COMMON/atomVector.h>
#include <BALL/MOLMEC/COMMON/forceField.h>

namespace BALL
{
	/** SLICK van-der-Waals Component based on AmberNonBonded \\
			{\bf Definition:} \URL{BALL/SCORING/COMPONENTS/vanDerWaalsSlick.h}
	*/
	class BALL_EXPORT VanDerWaalsSlick : public ScoringComponent
	{
		public:

		///
		enum CalculationMethod
		{
			/// Full Lennard-Jones 6-12 potential
			CALCULATION__FULL_LJ_POTENTIAL,

			/// Softened Lennard-Jones 6-12 potential (simple cut)
			CALCULATION__SOFTENED_LJ_POTENTIAL_SIMPLE,

			/// Log-Softened Lennard-Jones 6-12 potential
			CALCULATION__SOFTENED_LJ_POTENTIAL_LOG

		};


		///
		struct BALL_EXPORT Option
		{

			///
			static const String VERBOSITY;

			///
			static const String VDW_METHOD;

			///
			static const String VDW_CUT_ON;

			///
			static const String VDW_CUT_OFF;

			///
			static const String VDW_SOFTENING_LIMIT;

			///
			static const String LENNARD_JONES_FILE;

		};


		struct Default
		{

			///
			static const Size VERBOSITY;

			///
			static const Size VDW_METHOD;

			///
			static const float VDW_CUT_ON;

			///
			static const float VDW_CUT_OFF;

			///
			static const float VDW_SOFTENING_LIMIT;

			///
			static const String LENNARD_JONES_FILE;

		};



		///
		VanDerWaalsSlick()
			;

		///
		VanDerWaalsSlick(ScoringFunction& sf)
			;

		///
		VanDerWaalsSlick(ScoringComponent& sc)
			;

		///
		virtual ~VanDerWaalsSlick()
			;

		///
		virtual void clear()
			;

		/**
		 * Setup method
		 */
		virtual bool setup();

		void update(const vector<std::pair<Atom*, Atom*> >& pair_vector);

		/**
		 * Calculates and returns the component's energy.
		*/
		virtual double updateScore();


		protected:

		//_
		std::vector<LennardJones::Data> non_bonded_;

		//_
		std::vector<bool> is_hydrogen_bond_;

		Size number_of_1_4_;
		Size number_of_h_bonds_;

		LennardJones lennard_jones_;
		Potential1210 hydrogen_bond_;


		private:

		//_
		System vdw_system_;

		//_
		Molecule* vdw_receptor_;

		//_
		Molecule* vdw_ligand_;

		//_
		Size calculation_method_;

		//_
		float cut_on_vdw_;

		//_
		float cut_off_vdw_;

		//_
		float scaling_vdw_1_4_;

		//_
		float softening_limit_;

		//_
		double calculateVDWEnergy_(const AtomVector& atom_vector)
			;

		//_
		Size createNonBondedList_(const ForceField::PairVector& atom_pair_vector)
			;

		//_ Verbosity of the code
		Size verbosity_;

	};

}

#endif // BALL_SCORING_COMPONENTS_VANDERWAALSSLICK_H
