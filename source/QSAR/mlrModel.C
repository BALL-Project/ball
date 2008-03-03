 // -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// 

#include <BALL/QSAR/mlrModel.h>
#include <newmat/newmatap.h>
using namespace BALL::QSAR;


MLRModel::MLRModel(const QSARData& q) : LinearModel(q) 
{
	type_="MLR";
}

MLRModel::~MLRModel()
{
}

void MLRModel::train()
{	
	if(descriptor_matrix_.Ncols()==0)
	{
		throw Exception::InconsistentUsage(__FILE__,__LINE__,"Data must be read into the model before training!");
	}
	if(Y_.Ncols()==0)
	{
		throw Exception::InconsistentUsage(__FILE__,__LINE__,"No response values have been read! Model can not be trained!");
	}
	if (descriptor_matrix_.Ncols()>=descriptor_matrix_.Nrows())
	{	
		throw Exception::SingularMatrixError(__FILE__,__LINE__,"For MLR model, matrix must have more rows than columns in order to be invertible!!");
		//training_result_.ReSize(0,0);
		//return;
	}

  	Matrix m = descriptor_matrix_.t()*descriptor_matrix_;

		
	try
	{
		training_result_ = m.i()*descriptor_matrix_.t()*Y_;
	}
	catch(BaseException e)
	{
		cout<<e.what()<<endl;
		training_result_.ReSize(0,0);
		throw Exception::SingularMatrixError(__FILE__,__LINE__,"Matrix for MLR training is singular!! Check that descriptor_matrix_ does not contain empty columns!");
		return;
	}

	
}
