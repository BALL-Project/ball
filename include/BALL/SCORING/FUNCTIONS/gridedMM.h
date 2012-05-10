/* gridedMM.h
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

#ifndef BALL_SCORING_FUNCTIONS_GRIDEDMM_H
#define BALL_SCORING_FUNCTIONS_GRIDEDMM_H

#include <BALL/SCORING/COMMON/diffGridBasedScoring.h>
#include <BALL/MOLMEC/AMBER/amber.h>
#include <BALL/SCORING/COMPONENTS/electrostatic.h>

namespace BALL
{
	class BALL_EXPORT GridedMM : public DiffGridBasedScoring
	{
		public:
			GridedMM(AtomContainer& receptor, AtomContainer& ligand, Options& options);

			GridedMM(AtomContainer& receptor, Vector3& hashgrid_origin, Options& options);

		protected:
			AtomTypes& getAtomTypes();

			void setup();

			double getES();

			ForceFieldParameters forcefield_parameters_;
	};
}

#endif // BALL_SCORING_FUNCTIONS_GRIDEDMM_H
