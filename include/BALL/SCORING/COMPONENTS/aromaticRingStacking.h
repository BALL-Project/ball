// $id$

// THIS IS PRELIMINARY AND UNTESTED CODE! Use at your own risk.

#ifndef BALL_SCORING_COMPONENTS_AROMATIC_RING_STACKING_H
#define BALL_SCORING_COMPONENTS_AROMATIC_RING_STACKING_H

#include <BALL/SCORING/COMMON/scoringComponent.h>
#include <BALL/SCORING/COMMON/scoringFunction.h>
#include <BALL/DATATYPE/options.h>

#include <BALL/SCORING/COMPONENTS/CHPI.h>

namespace BALL
{

	/** 
	*/

	class AromaticRingStacking
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

		
		AromaticRingStacking()
			throw();

		AromaticRingStacking(ScoringFunction& sf)
			throw();

		AromaticRingStacking(const AromaticRingStacking& ars)
			throw();

		~AromaticRingStacking()
			throw();

		virtual bool setup()
			throw();

		virtual double  calculateScore()
			throw();

		private:

		std::vector< std::pair<const CHPI::AromaticRing*, const CHPI::AromaticRing*> > possible_interactions_;

		std::vector<CHPI::AromaticRing> receptor_rings_;

		std::vector<CHPI::AromaticRing> ligand_rings_;

		float f2f_plane_distance_lower_;
		float f2f_plane_distance_upper_;
		float f2f_lateral_displacemant_lower_;
		float f2f_lateral_displacemant_upper_;

		float f2e_center_distance_lower_;
		float f2e_center_distance_upper_;

		//_ The tolerance for judging angles to be "equal"
		float angle_tolerance_;

		//_ The tolerance area for creating scores instead of simply counted
		//_ interactions.
		float scoring_tolerance_;

	};

}


#endif // BALL_MOLMEC_SLICK_SLICK_AROMATIC_RING_STACKING_H
