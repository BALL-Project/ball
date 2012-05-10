/* diffScoringFunction.h
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

#ifndef BALL_SCORING_COMMON_DIFFSCORINGFUNCTION_H
#define BALL_SCORING_COMMON_DIFFSCORINGFUNCTION_H

#include <BALL/SCORING/COMMON/scoringFunction.h>
#include <BALL/MOLMEC/COMMON/forceField.h>


namespace BALL
{
	class BALL_EXPORT DifferentiableScoringFunction : public ScoringFunction
	{
		public:
			DifferentiableScoringFunction(AtomContainer& receptor, AtomContainer& ligand, Options& options);

			DifferentiableScoringFunction(AtomContainer& receptor, Vector3& hashgrid_origin, Options& options);

			/** updates forces of all ligand atoms and those receptor atoms that interact with the former ones. \n
			ForceField::updateForces() has to be overloaded in order to make sure that the forces of _receptor_ atoms are also reset to zero before starting the calculation anew !! */
// 			void updateForces();
//
// 			/** updates ONLY forces between receptor and ligand */
// 			void updateOnlyRecLigForces();

		private:
			//ForceField* force_field_;

	};
}

#endif // BALL_SCORING_COMMON_DIFFSCORINGFUNCTION_H
