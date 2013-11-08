#include <BALL/QSAR/bayesModel.h>

namespace BALL
{
	namespace QSAR
	{
		BayesModel::BayesModel(const QSARData& q)
			: ClassificationModel(q)
		{
			min_prob_diff_ = 0; 
			undef_act_class_id_ = 0;
		}

	}
}
