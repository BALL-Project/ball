/* PB.h
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
