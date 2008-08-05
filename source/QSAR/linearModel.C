// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
//  

#include <BALL/QSAR/linearModel.h>
//#include <newmatio.h>

using namespace BALL::QSAR;



LinearModel::LinearModel(const QSARData& q) : RegressionModel(q) {}

LinearModel::~LinearModel()
{
}

void LinearModel::operator=(const Model& m)
{
	Model::operator=(m);
	validation->setCoefficientStddev(((LinearModel*) &m)->validation->getCoefficientStddev());
}

RowVector LinearModel::predict(const vector<double>& substance, bool transform)
{
	if(training_result_.Ncols()==0)
	{
		throw Exception::InconsistentUsage(__FILE__,__LINE__,"Model must be trained before it can predict the activitiy of substances!");
	}

	RowVector v=getSubstanceVector(substance, transform);

	RowVector res=v*training_result_;

	if(transform && y_transformations_.Ncols()!=0)
	{
		backTransformPrediction(res);
	}
	
	return res;
}
