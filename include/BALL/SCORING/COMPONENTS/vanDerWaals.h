/* vdW.h
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

#ifndef BALL_SCORING_COMPONENTS_VANDERWAALS_H
#define BALL_SCORING_COMPONENTS_VANDERWAALS_H

#include <BALL/MOLMEC/AMBER/amberNonBonded.h>


namespace BALL
{
	class Electrostatic;

	class BALL_EXPORT VanDerWaals : public ScoringComponent
	{
		public :
			VanDerWaals(Options& options, ForceFieldParameters& forcefield_parameters);

			VanDerWaals(Electrostatic* es);

			~VanDerWaals();

			void update(const vector<std::pair<Atom*, Atom*> >& pair_vector);

			double updateScore();

			AmberNonBonded* getAmberNonBonded();

		private:
			bool do_calculations_;

			AmberNonBonded* amber_nb_;
	};
}


#endif // BALL_SCORING_COMPONENTS_VANDERWAALS_H
