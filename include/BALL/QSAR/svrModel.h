// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
//

#ifndef SVRMODEL
#define SVRMODEL

#ifndef KMODEL
#include <BALL/QSAR/kernelModel.h>
#endif

#ifndef NLMODEL
#include <BALL/QSAR/nonlinearModel.h>
#endif


namespace BALL
{
	namespace QSAR
	{
		class BALL_EXPORT SVRModel : public KernelModel
		{
			public:
				/** @name Constructors and Destructors
				 */
				//@{
				SVRModel(const QSARData& q, int k_type, double p1, double p2);

				~SVRModel();
				//@}
				
	
	
		};
	}
}


#endif // SVMMODEL
