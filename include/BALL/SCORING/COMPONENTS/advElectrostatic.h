/* advElectrostatic.h
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

#ifndef BALL_SCORING_COMPONENTS_ADVELECTROSTATIC_H
#define BALL_SCORING_COMPONENTS_ADVELECTROSTATIC_H

#include <BALL/SCORING/COMPONENTS/electrostatic.h>


namespace BALL
{
	class BALL_EXPORT AdvancedElectrostatic : public Electrostatic
	{
		public :
			AdvancedElectrostatic(ScoringFunction* sf, Options& options, ForceFieldParameters& forcefield_parameters);

			AdvancedElectrostatic(ScoringFunction* sf, VanDerWaals* vdw);

			double calculateDielectricConstant(Vector3& atom1, Vector3& atom2);

		private:
			float protein_dielectric_constant_;

			float solvate_dielectric_constant_;
	};
}

#endif // BALL_SCORING_COMPONENTS_ADVELECTROSTATIC_H
