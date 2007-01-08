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
			throw();

		///
		ScoringComponent(const ScoringComponent& sc)
			throw();

		///
		ScoringComponent(ScoringFunction& sf)
			throw();

		///
		virtual ~ScoringComponent()
			throw();

		///
		virtual void clear()
			throw();

		///
		virtual bool setup()
			throw();

		///
		ScoringFunction* getScoringFunction() const
			throw();

		///
		void setScoringFunction(ScoringFunction& sf)
			throw();

		///
		String getName() const
			throw();

		///
		void setName(const String& name)
			throw();

		///
		virtual double calculateScore()
			throw();

		///
		virtual double getScore() const
			throw();


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
