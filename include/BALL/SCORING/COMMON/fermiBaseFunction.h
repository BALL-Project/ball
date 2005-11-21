// $Id: fermiBaseFunction.h,v 1.1 2005/11/21 19:27:02 anker Exp $

#ifndef BALL_SCORING_COMMON_FERMIBASEFUNCTION
#define BALL_SCORING_COMMON_FERMIBASEFUNCTION

#include <BALL/SCORING/COMMON/baseFunction.h>

namespace BALL
{

	/** Sigmoidal base function
	 */
	class FermiBaseFunction
		: public ScoringBaseFunction
	{

		public:

			/// 
			FermiBaseFunction()
				throw();

			/// The function is defined in such a way that the score approaches 1
			/// for values < lower and 0 for values > upper. If you need contrary
			/// behaviour, you have to supply a "lower" which is greater than
			/// "upper". This will invert the function.
			FermiBaseFunction(float lower, float upper)
				throw();

			///
			void setLower(float lower)
				throw();

			///
			void setUpper(float upper)
				throw();

			///
			float calculate(float x) const
				throw();

			///
			float calculate(float x, float lower, float upper)
				throw();


		private:

			//_
			void computeFermiParameters_()
				throw();

			//_
			float a_;

			//_
			float b_;

	};

}
#endif // BALL_SCORING_COMMON_FERMIBASEFUNCTION
