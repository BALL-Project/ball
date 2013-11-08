// ----------------------------------------------------
// $Maintainer: Marcel Schumann $
// $Authors: Marcel Schumann $
// ----------------------------------------------------

#ifndef BALL_SCORING_FUNCTIONS_RESCORING4D_H
#define BALL_SCORING_FUNCTIONS_RESCORING4D_H

#include <BALL/SCORING/FUNCTIONS/rescoring3D.h>


namespace BALL
{
	class BALL_EXPORT Rescoring4D : public Rescoring3D
	{
		public:
			Rescoring4D(AtomContainer& receptor, AtomContainer& reference_ligand, Options& options, String free_energy_label, ScoringFunction* sf);

		protected:
			/** Obtain score-contributions for a single atom. Overloaded by class Rescoring4D */
			void generateAtomScoreContributions_(const Atom* atom, vector<double>& contributions);
	};
}

#endif // BALL_SCORING_FUNCTIONS_RESCORING4D_H
