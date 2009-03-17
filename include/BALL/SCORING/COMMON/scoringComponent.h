// $id$

#ifndef BALL_SCORING_COMMON_SCORINGCOMPONENT
#define BALL_SCORING_COMMON_SCORINGCOMPONENT

#include <BALL/DATATYPE/string.h>
#include <BALL/SCORING/COMMON/scoringFunction.h>

namespace BALL
{

	class ScoringFunction;

	class ScoringComponent
	{

		public:

		///
		ScoringComponent()
			;

		///
		ScoringComponent(const ScoringComponent& sc)
			;

		///
		ScoringComponent(ScoringFunction& sf)
			;

		///
		virtual ~ScoringComponent()
			;

		///
		virtual void clear()
			;

		///
		virtual bool setup()
			;

		///
		ScoringFunction* getScoringFunction() const
			;

		///
		void setScoringFunction(ScoringFunction& sf)
			;

		///
		String getName() const
			;

		///
		void setName(const String& name)
			;

		///
		virtual double calculateScore()
			;

		///
		virtual double getScore() const
			;


		protected:

		//_
		ScoringFunction* scoring_function_;
	
		//_
		double score_;


		private:

		//_
		String name_;

	};

}

#endif // BALL_SCORING_COMMON_SCORINGCOMPONENT
