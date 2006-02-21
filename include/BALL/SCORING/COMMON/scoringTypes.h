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
			throw();

		ScoringTypes(const ScoringTypes& st)
			throw();

		ScoringTypes(const ScoringComponent& sc)
			throw();

		virtual ~ScoringTypes()
			throw();

		virtual void clear()
			throw();

		virtual bool setup()
			throw();

		const HashMap<const Atom*, Size>& getTypeMap() const
			throw();

		protected:

		//_ The scoring component these types are bound to
		const ScoringComponent* scoring_component_;

		//_ The hash map containing the mapping of atom pointers to atom types
		HashMap<const Atom*, Size> type_map_;

	};

}

#endif // BALL_SCORING_COMMON_SCORINGTYPES
