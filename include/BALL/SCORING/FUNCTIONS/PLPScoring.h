// ----------------------------------------------------
// $Maintainer: Marcel Schumann $
// $Authors: Marcel Schumann $
// ----------------------------------------------------

#ifndef BALL_SCORING_FUNCTIONS_PLPSCORING_H
#define BALL_SCORING_FUNCTIONS_PLPSCORING_H

#include <BALL/SCORING/COMMON/scoringFunction.h>
#include <BALL/MOLMEC/MMFF94/MMFF94.h>

namespace BALL
{
	/** ScoringFunction using a grid-based version of Piecewise Linear Potential */
	class BALL_EXPORT PLPScoring : public ScoringFunction
	{
		public:
			PLPScoring(AtomContainer& receptor, AtomContainer& ligand, Options& options);

			PLPScoring(AtomContainer& receptor, Vector3& hashgrid_origin, Options& options);

			void setLigand(AtomContainer* sys);

		protected:
			AtomTypes& getAtomTypes();

			void setup();

			void setAtomType(Atom* atom, const String& type_name);

			double getES();

			AtomTypes atom_types_;
	};
}

#endif // BALL_SCORING_FUNCTIONS_PLPSCORING_H
