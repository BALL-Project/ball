/* MMScoring.h
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
