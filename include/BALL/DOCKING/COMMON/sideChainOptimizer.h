/* sideChainOptimizer.h
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

#ifndef BALL_DOCKING_COMMON_SIDECHAINOPTIMIZER
#define BALL_DOCKING_COMMON_SIDECHAINOPTIMIZER

#include <BALL/SCORING/COMMON/scoringFunction.h>
#include <BALL/STRUCTURE/rotamerLibrary.h>
#include <set>
#include <list>

namespace BALL
{
	class BALL_EXPORT SideChainOptimizer
	{
		public:
			SideChainOptimizer(ScoringFunction* sf);

			~SideChainOptimizer();

			void setFlexibleResidues(const set<String>& residues_IDs);

			void optimize();

			void findFlexibleSideChains(set<Residue*>& residues, const double& dist_cutoff, const double& min_B_factor);

		private:
			FragmentDB* frag_db_;

			RotamerLibrary* rotamer_lib_;

			ScoringFunction* scoring_function_;

			list<Residue*> residues_;
	};
}

#endif
