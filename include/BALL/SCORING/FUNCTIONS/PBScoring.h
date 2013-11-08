// ----------------------------------------------------
// $Maintainer: Marcel Schumann $
// $Authors: Marcel Schumann $
// ----------------------------------------------------

#ifndef BALL_SCORING_FUNCTIONS_PBSCORING_H
#define BALL_SCORING_FUNCTIONS_PBSCORING_H

#include <BALL/SCORING/COMMON/scoringFunction.h>


namespace BALL
{
	class BALL_EXPORT PBScoring : public ScoringFunction
	{
		public:
			/** constructor */
			PBScoring(AtomContainer& receptor, AtomContainer& reference_ligand, Options& options);

			PBScoring(AtomContainer& receptor, Vector3& hashgrid_origin, Options& options);

		protected:
			void setup();

			double getES();
	};
}

#endif // BALL_SCORING_FUNCTIONS_PBSCORING_H
