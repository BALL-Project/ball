  // -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
//

#ifndef KNNMODEL
#define KNNMODEL

#ifndef ALLMODEL
#include <BALL/QSAR/allModel.h>
#endif

namespace BALL
{
	namespace QSAR 
	{

		/** class for automated lazy learning (ALL-QSAR) */
		class KNNModel : public ALLModel
		{ 
			public:
				/** @name Constructors and Destructors
				 */
				//@{
				KNNModel(const QSARData& q, int k=3);
				//@}
				
				void calculateWeights(Matrix& dist, RowVector& w);
				
				void setParameters(vector<double>& v);
				
				vector<double> getParameters() const;
				
				bool optimizeParameters(int k, int no_steps);
				
			private:
				int k_ ;
		};
	}
}

#endif // KNNMODEL
