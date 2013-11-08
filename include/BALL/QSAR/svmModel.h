// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
//

#ifndef SVMMODEL
#define SVMMODEL

#ifndef CLASSIFICATION
#include <BALL/QSAR/classificationModel.h>
#endif

#ifndef KMODEL
#include <BALL/QSAR/kernelModel.h>
#endif

#ifndef NLMODEL
#include <BALL/QSAR/nonlinearModel.h>
#endif

namespace BALL{
	namespace QSAR {

		/** class for support vector classification */
		class BALL_EXPORT SVMModel : public ClassificationModel
		{ 
			public:
				/** @name Constructors and Destructors
				 */
				//@{
				SVMModel(const QSARData& q);

				virtual ~SVMModel();
				//@}			
				
				
		};
	}
}

#endif //SVMMODEL
