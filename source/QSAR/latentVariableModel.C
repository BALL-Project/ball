#include <BALL/QSAR/latentVariableModel.h>
#include <iostream.h>
using namespace BALL::QSAR;



LatentVariableModel::LatentVariableModel()
{
	latent_variables_.resize(0,0);
	loadings_.resize(0,0);
	weights_.resize(0,0);
}


const BALL::Matrix<double>* LatentVariableModel::getLoadings()
{
	return &loadings_;
}


const BALL::Matrix<double>* LatentVariableModel::getLatentVariables()
{
	return &latent_variables_;
}


const BALL::Matrix<double>* LatentVariableModel::getWeights()
{
	return &weights_;
}

