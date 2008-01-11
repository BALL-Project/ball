// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
//

#ifndef NLMODEL
#define NLMODEL

#ifndef REGRESSION
#include <BALL/QSAR/regressionModel.h>
#endif



namespace BALL
{
	namespace QSAR
	{
		class NonLinearModel : public RegressionModel
		{
			public:
				/** @name Constructors and Destructors
				 */
				//@{
				NonLinearModel(const QSARData& q);

				~NonLinearModel();
				//@}	
	
	
	
	
		};
	}
}


#endif // NLMODEL
