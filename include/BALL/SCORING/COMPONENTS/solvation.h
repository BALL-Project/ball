/* solvation.h
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

#ifndef BALL_SCORING_COMPONENTS_SOLVATION_H
#define BALL_SCORING_COMPONENTS_SOLVATION_H

#include <BALL/SCORING/COMMON/scoringFunction.h>
#include <map>


namespace BALL
{
	class BALL_EXPORT Solvation : public ScoringComponent
	{
		public:
			Solvation(ScoringFunction& sf);

			virtual void update(const AtomPairVector& pair_vector);

			virtual double updateScore();

		private:
			AtomPairVector pair_vector_;

		protected:
			map<String, double> solvation_parameters;
			map<String, double> solvation_volumes;
	};
}

#endif // BALL_SCORING_COMPONENTS_SOLVATION_H
