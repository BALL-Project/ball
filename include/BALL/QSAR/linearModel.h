// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// 

#ifndef LMODEL
#define LMODEL

#ifndef REGRESSION
#include <BALL/QSAR/regressionModel.h>
#endif

namespace BALL
{
	namespace QSAR
	{
		class BALL_EXPORT LinearModel : public RegressionModel
		{
		
			public: 
				/** @name Constructors and Destructors
				 */
				//@{
				/** constructur,
				@param q QSAR-wrapper object, from which the data for this model should be taken */
				LinearModel(const QSARData& q);
		
				~LinearModel();
				
				virtual void operator=(const LinearModel& m);
				
				//@}
				
				
				/** @name Accessors
				 */
				//@{
				virtual Eigen::VectorXd predict(const vector<double>& substance, bool transform=1);
	
			protected:
				void calculateOffsets();
				//@}
		};
	}
}

#endif // LMODEL
