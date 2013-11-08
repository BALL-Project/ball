// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
//  

#include <BALL/QSAR/linearModel.h>

namespace BALL
{
	namespace QSAR
	{

		LinearModel::LinearModel(const QSARData& q) : RegressionModel(q) {}

		LinearModel::~LinearModel()
		{
		}

		void LinearModel::operator = (const LinearModel& m)
		{
			RegressionModel::operator = (m);
			validation->setCoefficientStdErrors(m.validation->getCoefficientStdErrors());
		}


		void LinearModel::calculateOffsets()
		{
			offsets_ = ((descriptor_matrix_*training_result_)-Y_).colwise().sum() / training_result_.rows();
			//cout<<"offset : "<<offsets_(1)<<endl<<flush;
		}


		Eigen::VectorXd LinearModel::predict(const vector<double> & substance, bool transform)
		{
			if (training_result_.rows() == 0)
			{
				throw Exception::InconsistentUsage(__FILE__, __LINE__, "Model must be trained before it can predict the activitiy of substances!"); 
			}

			Eigen::VectorXd v = getSubstanceVector(substance, transform); 

			Eigen::VectorXd res = v.transpose()*training_result_;
			//if (offsets_.getSize() == res.getSize()) res -= offsets_; 

			if (transform && y_transformations_.cols() != 0)
			{
				backTransformPrediction(res); 
			}
			
			return res;
		}
	}
}
