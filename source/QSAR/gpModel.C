// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// 

#include <BALL/QSAR/gpModel.h>
#include <newmatio.h>
using namespace BALL::QSAR; 



GPModel::GPModel(const QSARData& q, int k_type, double p1, double p2) : KernelModel(q,k_type,p1,p2) 
{
	type_="GP";
	lambda_ = 0.001;
}

GPModel::GPModel(const QSARData& q, RowVector& w) : KernelModel(q,w) 
{
	type_="GP";
	lambda_ = 0.001;
}


GPModel::GPModel(const QSARData& q, String s1, String s2) : KernelModel(q,s1,s2) 
{
	type_="GP";
	lambda_ = 0.001;
}

GPModel::GPModel(const QSARData& q, const LinearModel& lm, int column) : KernelModel(q,lm, column) 
{
	type_="GP";
	lambda_ = 0.001;
}

GPModel::~GPModel()
{
}


void GPModel::train()
{
	if(descriptor_matrix_.Ncols()==0)
	{
		throw Exception::InconsistentUsage(__FILE__,__LINE__,"Data must be read into the model before training!");
	}
	kernel->calculateKernelMatrix(descriptor_matrix_, K_);
	IdentityMatrix I(K_.Nrows());
	
	try
	{
		L_ = (K_+I*pow(lambda_,2)).i();  // dim: nxn
	}
	catch(BaseException e)
	{
		cout<<e.what()<<endl;
		L_.ReSize(0,0);
		throw Exception::SingularMatrixError(__FILE__,__LINE__,"Matrix for GP training is singular!! Check that descriptor_matrix_ does not contain empty columns and/or lambda is large enough!");
		return;
	}

// 	double l = pow(lambda_,2);
// 	addLambda(K_,l);
// 	L_ = K_.i();
	
	training_result_ = L_*Y_; // B: one coefficient for each substance; dim: nxc
	
	calculateOffsets();
}


RowVector GPModel::predict(const vector<double>& substance, bool transform)
{
	if(training_result_.Ncols()==0)
	{
		throw Exception::InconsistentUsage(__FILE__,__LINE__,"Model must be trained before it can predict the activitiy of substances!");
	}
	input_=getSubstanceVector(substance,transform);
	
	kernel->calculateKernelMatrix(K_, input_, descriptor_matrix_, K_t_); // dim: 1xn
	RowVector res = K_t_*training_result_; // dim: 1xc
	
	if(transform && y_transformations_.Ncols()!=0)
	{
		backTransformPrediction(res);
	}
	return res;	
}


double GPModel::calculateStdErr()
{
	Matrix mx;
	kernel->calculateKernelMatrix(input_,mx); // k(x*,x*), dim: 1x1
	
	double sum=0;
	for(int i=1; i<=K_t_.Ncols();i++)
	{
		for(int j=1; j<=K_t_.Ncols();j++)
		{
			sum+=K_t_(1,i)*K_t_(1,j)*L_(i,j); // k(x*,_i)*k(x*,x_j)*L_ij
		}
	}
	double res=sqrt(abs(mx.AsScalar()-sum));
	return res;	
}


void GPModel::setParameters(vector<double>& v)
{
	if(v.size()!=1)
	{
		String c = "Wrong number of model parameters! Needed: 1;";
		c = c+" given: "+String(v.size());
		throw Exception::ModelParameterError(__FILE__,__LINE__,c.c_str());
	}
	lambda_ = v[0];
}

vector<double> GPModel::getParameters() const
{
	vector<double> d;
	d.push_back(lambda_);
	return d;
}
