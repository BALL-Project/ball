 // -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// 

#include <BALL/QSAR/mlrModel.h>
#include <newmat/newmatap.h>
using namespace BALL::QSAR;


MLRModel::MLRModel(const QSARData& q, double ridge_regression) : LinearModel(q) 
{
	type_="MLR";
	rr_=ridge_regression;
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
	if (rr_==0 && descriptor_matrix_.Ncols()>=descriptor_matrix_.Nrows())
	{	
		throw Exception::SingularMatrixError(__FILE__,__LINE__,"For MLR model, matrix must have more rows than columns in order to be invertible!!");
		//training_result_.ReSize(0,0);
		//return;
	}

  	Matrix m = descriptor_matrix_.t()*descriptor_matrix_;

	if(rr_ != 0)
	{
		IdentityMatrix im(m.Nrows());
		im=im*rr_;
		m=m+im;
	}
		
	try{
		training_result_ = m.i()*descriptor_matrix_.t()*Y_;
		
// 		UpperTriangularMatrix R;
// 		Matrix X1 = descriptor_matrix_;
// 		Matrix Y1=Y_;
//  		Matrix M;
// 		QRZ(X1,R); 
// 		QRZ(X1, Y1, M);
// 		training_result_ = R.i()*M;

	}
	catch(BaseException e)
	{
		cout<<e.what()<<endl;
		training_result_.ReSize(0,0);
		throw Exception::SingularMatrixError(__FILE__,__LINE__,"Matrix for MLR training is singular!! Check that descriptor_matrix_ does not contain empty columns!");
		return;
	}

	
}

void MLRModel::setParameters(vector<double>& v)
{	
	if(v.size()!=1)
	{
		String c = "Wrong number of model parameters! Needed: 1;";
		c = c+" given: "+String(v.size());
		throw Exception::ModelParameterError(__FILE__,__LINE__,c.c_str());
	}
	rr_=v[0];
}

vector<double> MLRModel::getParameters() const
{
	vector<double> d;
	d.push_back(rr_);
	return d;
}
