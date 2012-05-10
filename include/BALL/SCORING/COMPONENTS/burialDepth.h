/* burialDepth.h
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

#ifndef BALL_SCORING_COMPONENTS_BURIALDEPTH_H
#define BALL_SCORING_COMPONENTS_BURIALDEPTH_H

#include <BALL/SCORING/COMMON/scoringComponent.h>
#include <BALL/SCORING/COMMON/scoringFunction.h>


namespace BALL
{
	class BALL_EXPORT BurialDepth : public ScoringComponent
	{
		public :
			BurialDepth(ScoringFunction& sf);

			~BurialDepth();

			void update(const vector<std::pair<Atom*, Atom*> >& pair_vector);

			double updateScore();
	};
}

#endif // BALL_SCORING_COMPONENTS_BURIALDEPTH_H
