#include <BALL/QSAR/latentVariableModel.h>
#include <iostream>

namespace BALL
{
	namespace QSAR
	{

		LatentVariableModel::LatentVariableModel()
		{
			latent_variables_.resize(0, 0);
			loadings_.resize(0, 0);
			weights_.resize(0, 0);
		}


		const Eigen::MatrixXd* LatentVariableModel::getLoadings()
		{
			return &loadings_;
		}


		const Eigen::MatrixXd* LatentVariableModel::getLatentVariables()
		{
			return &latent_variables_;
		}


		const Eigen::MatrixXd* LatentVariableModel::getWeights()
		{
			return &weights_;
		}

	}
}
