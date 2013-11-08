// ----------------------------------------------------
// $Maintainer: Marcel Schumann $
// $Authors: Slick-development Team, Marcel Schumann $
// ----------------------------------------------------

#ifndef BALL_SCORING_COMPONENTS_AROMATICRINGSTACKING_H
#define BALL_SCORING_COMPONENTS_AROMATICRINGSTACKING_H

#include <BALL/QSAR/aromaticityProcessor.h>
#include <BALL/QSAR/ringPerceptionProcessor.h>
#include <BALL/SCORING/COMMON/scoringComponent.h>
#include <BALL/SCORING/COMMON/scoringFunction.h>
#include <BALL/DATATYPE/options.h>

#include <BALL/SCORING/COMPONENTS/CHPI.h>

namespace BALL
{
	class BALL_EXPORT AromaticRingStacking
		:	public ScoringComponent
	{
		public:
			struct Option
			{
				static const String VERBOSITY;
				static const String F2F_PLANE_DISTANCE_LOWER;
				static const String F2F_PLANE_DISTANCE_UPPER;
				static const String F2F_LATERAL_DISPLACEMENT_LOWER;
				static const String F2F_LATERAL_DISPLACEMENT_UPPER;
				static const String F2E_CENTER_DISTANCE_LOWER;
				static const String F2E_CENTER_DISTANCE_UPPER;
				static const String SCORING_TOLERANCE;
			};

			struct Default
			{
				static const Size VERBOSITY;
				static const float F2F_PLANE_DISTANCE_LOWER;
				static const float F2F_PLANE_DISTANCE_UPPER;
				static const float F2F_LATERAL_DISPLACEMENT_LOWER;
				static const float F2F_LATERAL_DISPLACEMENT_UPPER;
				static const float F2E_CENTER_DISTANCE_LOWER;
				static const float F2E_CENTER_DISTANCE_UPPER;
				static const float SCORING_TOLERANCE;
			};

			AromaticRingStacking();

			AromaticRingStacking(ScoringFunction& sf);

			AromaticRingStacking(const AromaticRingStacking& ars);

			~AromaticRingStacking();

			void clear();

			virtual bool setup(Options& options);

			/** This function needs to be called once for every new ligand */
			void setupLigand();

			void update(const vector<std::pair<Atom*, Atom*> >& /*atom_pairs*/);

			virtual double updateScore();

		private:
			/** A TimeStamp that is used to check during each call of update whether the receptor has been changed.\n
			If this is the case, the ring-centers and normal-vector will be recomputed. */
			TimeStamp update_time_stamp_;

			std::vector< std::pair<const CHPI::AromaticRing*, const CHPI::AromaticRing*> > possible_interactions_;

			std::vector<CHPI::AromaticRing*> receptor_rings_;

			std::vector<CHPI::AromaticRing*> ligand_rings_;

			float f2f_plane_distance_lower_;
			float f2f_plane_distance_upper_;
			float f2f_lateral_displacemant_lower_;
			float f2f_lateral_displacemant_upper_;
			float f2e_center_distance_lower_;
			float f2e_center_distance_upper_;

			//_ The tolerance for judging angles to be "equal"
			float angle_tolerance_;

			float distance_cutoff_;

			//_ The tolerance area for creating scores instead of simply counted
			//_ interactions.
			float scoring_tolerance_;

			/** This component is valid only if setup() has been run and the receptor has thus been processed */
			bool valid_;

			RingPerceptionProcessor rp_;
			AromaticityProcessor ap_;
	};
}

#endif // BALL_SCORING_COMPONENTS_AROMATICRINGSTACKING_H
