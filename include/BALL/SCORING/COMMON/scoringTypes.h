#ifndef BALL_SCORING_COMMON_SCORINGTYPES
#define BALL_SCORING_COMMON_SCORINGTYPES

#include <BALL/SCORING/COMMON/scoringComponent.h>

namespace BALL
{

	/** Define the interface for the base function
	 */
	class ScoringTypes
	{

		public:

		ScoringTypes()
			;

		ScoringTypes(const ScoringTypes& st)
			;

		ScoringTypes(const ScoringComponent& sc)
			;

		virtual ~ScoringTypes()
			;

		virtual void clear()
			;

		virtual bool setup()
			;

		const HashMap<const Atom*, Size>& getTypeMap() const
			;

		protected:

		//_ The scoring component these types are bound to
		const ScoringComponent* scoring_component_;

		//_ The hash map containing the mapping of atom pointers to atom types
		HashMap<const Atom*, Size> type_map_;

	};

}

#endif // BALL_SCORING_COMMON_SCORINGTYPES
