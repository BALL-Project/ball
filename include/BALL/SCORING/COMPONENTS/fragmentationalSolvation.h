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
