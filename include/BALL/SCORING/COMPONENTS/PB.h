// ----------------------------------------------------
// $Maintainer: Marcel Schumann $
// $Authors: Marcel Schumann $
// ----------------------------------------------------

#include <BALL/SCORING/COMMON/scoringComponent.h>
#include <BALL/SOLVATION/poissonBoltzmann.h>

#ifndef BALL_SCORING_COMPONENTS_PB_H
#define BALL_SCORING_COMPONENTS_PB_H

namespace BALL
{
	/** Poisson-Boltzmann scoring term */
	class BALL_EXPORT PB : public ScoringComponent
	{
		public :
			PB(ScoringFunction& sf);

			~PB();

			void update(const vector<std::pair<Atom*, Atom*> >& pair_vector);

			double updateScore();

			void setupLigand();

		protected:
			FDPB* pb_solver_;

			System receptor_atoms_;
			System ligand_atoms_;
			System complex_atoms_;
	};
}

#endif // BALL_SCORING_COMPONENTS_PB_H
