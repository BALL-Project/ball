// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
//

#ifndef BALL_QSAR_BAYESMODEL_H
#define BALL_QSAR_BAYESMODEL_H

#ifndef BALL_QSAR_CLASSIFICATIONMODEL_H
#include <BALL/QSAR/classificationModel.h>
#endif

namespace BALL
{
	namespace QSAR
	{
		/** interface class for all Bayes classification models */
		class BALL_EXPORT BayesModel : public ClassificationModel
		{
			public:

				BayesModel(const QSARData& q);

				virtual bool isTrained() = 0;

				/** calculate the probability for the specified feature to assume the given value for each class
				@return a probability for each class */
				virtual vector<double> calculateProbabilities(int activitiy_index, int feature_index, double feature_value) = 0;

				/** returns the number of response variables for which this model has been trained */
				virtual int getNoResponseVariables() = 0;


			protected:
				/** the minimal desired difference between the predicted class and the second best class. If the difference is smaller than this value, undef_act_class_id_ should be used instead.*/
				double min_prob_diff_;

				/** the class_id that is to be assigned, if no class could be savely predicted, if difference between best and second best potential class is smaller than min_prob_diff_ */
				double undef_act_class_id_;

		};
	}
}

#endif //BAYESMODEL_H
