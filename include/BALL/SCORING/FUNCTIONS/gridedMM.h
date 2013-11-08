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
