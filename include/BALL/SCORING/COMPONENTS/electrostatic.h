// ----------------------------------------------------
// $Maintainer: Marcel Schumann $
// $Authors: Marcel Schumann $
// ----------------------------------------------------

#ifndef BALL_SCORING_COMPONENTS_ELECTROSTATIC_H
#define BALL_SCORING_COMPONENTS_ELECTROSTATIC_H

#include <BALL/MOLMEC/AMBER/amberNonBonded.h>
#include <BALL/SCORING/COMMON/scoringComponent.h>

namespace BALL
{
	class VanDerWaals;

	class BALL_EXPORT Electrostatic : public ScoringComponent
	{
		public :
			Electrostatic(Options& options, ForceFieldParameters& forcefield_parameters);

			Electrostatic(VanDerWaals* vdw);

			~Electrostatic();

			void update(const vector<std::pair<Atom*, Atom*> >& pair_vector);

			double updateScore();

			AmberNonBonded* getAmberNonBonded();

		private:
			bool do_calculations_;

		protected:
			AmberNonBonded* amber_nb_;
	};
}

#endif // BALL_SCORING_COMPONENTS/ELECTROSTATIC_H
