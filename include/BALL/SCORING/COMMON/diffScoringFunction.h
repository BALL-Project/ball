// ----------------------------------------------------
// $Maintainer: Marcel Schumann $
// $Authors: Marcel Schumann $
// ----------------------------------------------------

#ifndef BALL_SCORING_COMMON_DIFFSCORINGFUNCTION_H
#define BALL_SCORING_COMMON_DIFFSCORINGFUNCTION_H

#include <BALL/SCORING/COMMON/scoringFunction.h>
#include <BALL/MOLMEC/COMMON/forceField.h>


namespace BALL
{
	class BALL_EXPORT DifferentiableScoringFunction : public ScoringFunction
	{
		public:
			DifferentiableScoringFunction(AtomContainer& receptor, AtomContainer& ligand, Options& options);

			DifferentiableScoringFunction(AtomContainer& receptor, Vector3& hashgrid_origin, Options& options);

			/** updates forces of all ligand atoms and those receptor atoms that interact with the former ones. \n
			ForceField::updateForces() has to be overloaded in order to make sure that the forces of _receptor_ atoms are also reset to zero before starting the calculation anew !! */
// 			void updateForces();
//
// 			/** updates ONLY forces between receptor and ligand */
// 			void updateOnlyRecLigForces();

		private:
			//ForceField* force_field_;

	};
}

#endif // BALL_SCORING_COMMON_DIFFSCORINGFUNCTION_H
