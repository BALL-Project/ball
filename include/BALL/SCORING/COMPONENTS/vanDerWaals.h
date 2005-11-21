// $Id: vanDerWaals.h,v 1.1 2005/11/21 19:27:06 anker Exp $
// Molecular Mechanics: SLICK force field, modified van-der-Waals term

#ifndef BALL_SCORING_COMPONENTS_VANDERWAALS_H
#define BALL_SCORING_COMPONENTS_VANDERWAALS_H

#include <BALL/SCORING/COMMON/scoringComponent.h>
#include <BALL/MOLMEC/AMBER/amberNonBonded.h>
#include <BALL/MOLMEC/PARAMETER/lennardJones.h>
#include <BALL/MOLMEC/COMMON/atomVector.h>
#include <BALL/MOLMEC/COMMON/forceField.h>

namespace BALL
{
	/** SLICK van-der-Waals Component based on AmberNonBonded \\
			{\bf Definition:} \URL{BALL/SCORING/COMPONENTS/vanDerWaals.h}
	*/
	class VanDerWaals
		:	public ScoringComponent
	{

		public:

		struct Option
		{

			///
			static const String LENNARD_JONES_FILE;

		};


		struct Default
		{

			///
			static const String LENNARD_JONES_FILE;

		};



		///
		VanDerWaals()
			throw();

		///
		VanDerWaals(ScoringFunction& sf)
			throw();

		///
		VanDerWaals(ScoringComponent& sc)
			throw();

		///
		virtual ~VanDerWaals()
			throw();

		///
		virtual void clear()
			throw();

		///
		virtual bool setup()
			throw();

		///
		virtual double calculateScore()
			throw();


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
		double calculateVDWEnergy_(const AtomVector& atom_vector)
			throw();

		//_
		Size createNonBondedList_(const ForceField::PairVector& atom_pair_vector)
			throw();

	};

}

#endif // BALL_SCORING_COMPONENTS_VANDERWAALS_H
