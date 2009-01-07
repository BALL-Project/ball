#include <BALL/QSAR/bayesModel.h>

using namespace BALL::QSAR;


BayesModel::BayesModel(const QSARData& q)
	: ClassificationModel(q)
{
	min_prob_diff_ = 0;
	undef_act_class_id_ = 0;
}

