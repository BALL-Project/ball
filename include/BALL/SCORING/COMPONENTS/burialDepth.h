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
