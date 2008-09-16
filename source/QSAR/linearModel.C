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


void LinearModel::calculateOffsets()
{
	Matrix<double> residuals = (descriptor_matrix_*training_result_)-Y_;
	int no_act=training_result_.Ncols();
	offsets_.resize(no_act);
	offsets_.setVectorType(0); // row-vector
	for(int i=1; i<=no_act; i++)
	{	
		offsets_(i) = residuals.colSum(i) / training_result_.Nrows();
	}
	//cout<<"offset : "<<offsets_(1)<<endl<<flush;
}


BALL::Vector<double> LinearModel::predict(const vector<double>& substance, bool transform)
{
	if(training_result_.getSize()==0)
	{
		throw Exception::InconsistentUsage(__FILE__,__LINE__,"Model must be trained before it can predict the activitiy of substances!");
	}

	Vector<double> v=getSubstanceVector(substance, transform);

	Vector<double> res=v*training_result_;

	if(transform && y_transformations_.Ncols()!=0)
	{
		backTransformPrediction(res);
	}
	
	if(offsets_.getSize()==res.getSize()) res -= offsets_;
	
	return res;
}
