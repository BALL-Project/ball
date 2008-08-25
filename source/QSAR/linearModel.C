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
	Matrix residuals = (descriptor_matrix_*training_result_)-Y_;
	int no_act=training_result_.Ncols();
	offsets_.ReSize(no_act);
	for(int i=1; i<=no_act; i++)
	{	
		offsets_(i) = residuals.Column(i).Sum() / training_result_.Nrows();
	}
	//cout<<"offset : "<<offsets_(1)<<endl<<flush;
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
	
	if(offsets_.Ncols()==res.Ncols()) res -= offsets_;
	
	return res;
}
