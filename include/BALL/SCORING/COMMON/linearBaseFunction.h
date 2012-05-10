// $Id: linearBaseFunction.h,v 1.1 2005/11/21 19:27:03 anker Exp $

#ifndef BALL_SCORING_COMMON_LINEARBASEFUNCTION_H
#define BALL_SCORING_COMMON_LINEARBASEFUNCTION_H

#include <BALL/SCORING/COMMON/baseFunction.h>

namespace BALL
{

	/** Linear base function
	 */
	class BALL_EXPORT LinearBaseFunction
		: public ScoringBaseFunction
	{

		public:

			///
			LinearBaseFunction()
				;

			/// The function is defined in such a way that the score is 1 for
			/// values < lower and 0 for values > upper. If you need contrary 
			/// behaviour, you have to supply a "lower" which is greater than
			/// "upper". This will invert the function.
			LinearBaseFunction(float lower, float upper)
				;

			///
			float calculate(float x) const
				;

			///
			float calculate(float x, float lower, float upper)
				;

	};

}

#endif // BALL_SCORING_COMMON_LINEARBASEFUNCTION_H
