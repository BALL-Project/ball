// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//

#ifndef BALL_DOCKING_COMMON_SIDECHAINOPTIMIZER
#define BALL_DOCKING_COMMON_SIDECHAINOPTIMIZER

#ifndef BALL_SCORING_COMMON_SCORINGFUNCTION_H
#include <BALL/SCORING/COMMON/scoringFunction.h>
#endif

#ifndef BALL_STRUCTURE_ROTAMERLIBRARY_H
#include <BALL/STRUCTURE/rotamerLibrary.h>
#endif

#include <set>
#include <list>

namespace BALL
{
	class BALL_EXPORT SideChainOptimizer
	{
		public:
			SideChainOptimizer(ScoringFunction* sf);

			~SideChainOptimizer();

			void setFlexibleResidues(const std::set<String>& residues_IDs);

			void optimize();

			void findFlexibleSideChains(std::set<Residue*>& residues, const double& dist_cutoff, const double& min_B_factor);

		private:
			FragmentDB* frag_db_;

			RotamerLibrary* rotamer_lib_;

			ScoringFunction* scoring_function_;

			list<Residue*> residues_;
	};
}

#endif
