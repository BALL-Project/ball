#include <BALL/QSAR/latentVariableModel.h>
 
using namespace BALL::QSAR;



LatentVariableModel::LatentVariableModel()
{
	latent_variables_.ReSize(0,0);
	loadings_.ReSize(0,0);
	weights_.ReSize(0,0);
}


const Matrix* LatentVariableModel::getLoadings()
{
	return &loadings_;
}


const Matrix* LatentVariableModel::getLatentVariables()
{
	return &latent_variables_;
}


const Matrix* LatentVariableModel::getWeights()
{
	return &weights_;
}

