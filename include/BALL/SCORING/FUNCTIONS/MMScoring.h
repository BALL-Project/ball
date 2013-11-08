// ----------------------------------------------------
// $Maintainer: Marcel Schumann $
// $Authors: Marcel Schumann $
// ----------------------------------------------------

#ifndef BALL_SCORING_FUNCTIONS_MMSCORING_H
#define BALL_SCORING_FUNCTIONS_MMSCORING_H

#include <BALL/SCORING/COMMON/diffScoringFunction.h>
#include <BALL/MOLMEC/AMBER/amber.h>


namespace BALL
{
	class BALL_EXPORT MMScoring : public DifferentiableScoringFunction
	{
		public:
			/** constructor */
			MMScoring(AtomContainer& receptor, AtomContainer& reference_ligand, Options& options);

			MMScoring(AtomContainer& receptor, Vector3& hashgrid_origin, Options& options);

		protected:
			void setup();

			double getES();

			/** saving the ForceFieldParameters here allow access to the atom-types and prevents multiple reading of the same parameter file in case of mulitple force-fields components */
			ForceFieldParameters forcefield_parameters_;
	};
}

#endif // BALL_SCORING_FUNCTIONS_MMSCORING_H
