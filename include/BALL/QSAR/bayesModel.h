// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
//


#ifndef BAYESMODEL
#define BAYESMODEL

#include <BALL/QSAR/classificationModel.h>


namespace BALL
{
	namespace QSAR 
	{
		/** interface class for all Bayes classification models */
		class BayesModel : public ClassificationModel
		{ 
			public: 
				
			BayesModel(const QSARData& q);
			
			virtual bool isTrained() = 0;
			
			/** calculate the probability for the specified feature to assume the given value for each class
			@return a probability for each class */
			virtual vector<double> calculateProbabilities(int activitiy_index, int feature_index, double feature_value) = 0;
	
		};
	}
}

#endif //BAYESMODEL
