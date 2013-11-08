// ----------------------------------------------------
// $Maintainer: Marcel Schumann $
// $Authors: Marcel Schumann $
// ----------------------------------------------------

#ifndef BALL_SCORING_COMPONENTS_VANDERWAALS_H
#define BALL_SCORING_COMPONENTS_VANDERWAALS_H

#include <BALL/MOLMEC/AMBER/amberNonBonded.h>


namespace BALL
{
	class Electrostatic;

	class BALL_EXPORT VanDerWaals : public ScoringComponent
	{
		public :
			VanDerWaals(Options& options, ForceFieldParameters& forcefield_parameters);

			VanDerWaals(Electrostatic* es);

			~VanDerWaals();

			void update(const vector<std::pair<Atom*, Atom*> >& pair_vector);

			double updateScore();

			AmberNonBonded* getAmberNonBonded();

		private:
			bool do_calculations_;

			AmberNonBonded* amber_nb_;
	};
}


#endif // BALL_SCORING_COMPONENTS_VANDERWAALS_H
