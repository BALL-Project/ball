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
		class BALL_EXPORT NonLinearModel : public RegressionModel
		{
			public:
				/** @name Constructors and Destructors
				 */
				//@{
				NonLinearModel(const QSARData& q);

				~NonLinearModel();
				
			protected:
				/** do nothing for non-kernel models. KernelModel on the other hand overloads this function... */
				virtual void calculateOffsets() {};
				//@}	
	
	
	
	
		};
	}
}


#endif // NLMODEL
