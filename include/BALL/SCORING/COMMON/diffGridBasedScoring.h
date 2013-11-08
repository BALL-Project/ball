// ----------------------------------------------------
// $Maintainer: Marcel Schumann $
// $Authors: Marcel Schumann $
// ----------------------------------------------------

#ifndef BALL_SCORING_COMMON_DIFFGRIDBASEDSCORING_H
#define BALL_SCORING_COMMON_DIFFGRIDBASEDSCORING_H

#include <BALL/SCORING/COMMON/gridBasedScoring.h>
#include <BALL/MOLMEC/COMMON/forceField.h>


namespace BALL
{
	class BALL_EXPORT DiffGridBasedScoring : public GridBasedScoring
	{
		public:
			DiffGridBasedScoring(AtomContainer& receptor, AtomContainer& ligand, Options& options);

			DiffGridBasedScoring(AtomContainer& receptor, Vector3& hashgrid_origin, Options& options);

			~DiffGridBasedScoring();

			virtual void setLigand(AtomContainer* sys);

			/** Overloads ScoringFunction::update() so that receptor-ligand ScoringComponents will not be updated (because grid will be used instead) */
			void update();

			/** Overloads ScoringFunction::updateScore() so that the grid score is used to asses the current receptor-ligand interaction */
			virtual double updateScore();

			void testOverlaps(Vector3& position, HashGrid3<Atom*>* hashg = NULL);

		protected:
			//ForceField* force_field_;

			/** update pairs and precalculates a score for one cell of a ScoreGrids
			@param set the id of the ScoreGrid, whose HashGrid is to be used */
			void updatePrecalculatedScore(Size set);

		private:

	};
}

#endif // BALL_SCORING_COMMON_DIFFGRIDBASEDSCORING_H
