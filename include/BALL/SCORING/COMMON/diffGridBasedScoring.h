/* diffGridBasedScoring.h
*
* Copyright (C) 2011 Marcel Schumann
*
* This program free software; you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation; either version 3 of the License, or (at
* your option) any later version.
*
* This program is distributed in the hope that it will be useful, but
* WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
* General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with this program; if not, see <http://www.gnu.org/licenses/>.
*/

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
