// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
//

#ifndef BALL_QSAR_KNNMODEL_H
#define BALL_QSAR_KNNMODEL_H

#ifndef BALL_QSAR_ALLMODEL_H
#include <BALL/QSAR/allModel.h>
#endif

namespace BALL
{
	namespace QSAR
	{

		/** class for automated lazy learning (ALL-QSAR) */
		class BALL_EXPORT KNNModel : public ALLModel
		{ 
			public:
				/** @name Constructors and Destructors
				 */
				//@{
				KNNModel(const QSARData& q, int k=3);
				//@}
				
				void calculateWeights(Eigen::MatrixXd& dist, Eigen::VectorXd& w);
				
				void setParameters(vector<double>& v);
				
				vector<double> getParameters() const;
				
				bool optimizeParameters(int k, int no_steps);
				
			private:
				int k_ ;
		};
	}
}

#endif // KNNMODEL
