/* fragmentationalSolvation.h
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

#ifndef BALL_SCORING_COMPONENTS_FRAGMENTATIONALSOLVATION_H
#define BALL_SCORING_COMPONENTS_FRAGMENTATIONALSOLVATION_H

#include <BALL/SCORING/COMPONENTS/solvation.h>


namespace BALL
{
	class BALL_EXPORT FragmentationalSolvation : public Solvation
	{
		public:
			FragmentationalSolvation(ScoringFunction& sf);

			void update(const AtomPairVector& pair_vector);

			double updateScore();

		private:
			double getDesolvatedFraction(const Atom* it, const HashGrid3<Atom*>* hashgrid);

			Vector3 getAtomCell(Atom* atom, const HashGrid3<Atom*>* hashgrid);
	};
}

#endif // BALL_SCORING_COMPONENTS_FRAGMENTATIONALSOLVATION_H
