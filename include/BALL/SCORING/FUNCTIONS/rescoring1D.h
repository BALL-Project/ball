// ----------------------------------------------------
// $Maintainer: Marcel Schumann $
// $Authors: Marcel Schumann $
// ----------------------------------------------------

#ifndef BALL_SCORING_FUNCTIONS_RESCORING1D_H
#define BALL_SCORING_FUNCTIONS_RESCORING1D_H

#include <BALL/SCORING/FUNCTIONS/rescoring.h>

namespace BALL
{
	class BALL_EXPORT Rescoring1D : public Rescoring
	{
		public:
			Rescoring1D(AtomContainer& receptor, AtomContainer& reference_ligand, Options& options, String free_energy_label, ScoringFunction* sf);

		protected:
			void setup_();
			const Protein* protein_;
			void generateScoreContributions_(vector<vector<double> >* matrix, vector<double>* v);
	};
}

#endif
