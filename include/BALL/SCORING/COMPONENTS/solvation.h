// ----------------------------------------------------
// $Maintainer: Marcel Schumann $
// $Authors: Marcel Schumann $
// ----------------------------------------------------

#ifndef BALL_SCORING_COMPONENTS_SOLVATION_H
#define BALL_SCORING_COMPONENTS_SOLVATION_H

#include <BALL/SCORING/COMMON/scoringFunction.h>
#include <map>


namespace BALL
{
	class BALL_EXPORT Solvation : public ScoringComponent
	{
		public:
			Solvation(ScoringFunction& sf);

			virtual void update(const AtomPairVector& pair_vector);

			virtual double updateScore();

		private:
			AtomPairVector pair_vector_;

		protected:
			std::map<String, double> solvation_parameters;
			std::map<String, double> solvation_volumes;
	};
}

#endif // BALL_SCORING_COMPONENTS_SOLVATION_H
