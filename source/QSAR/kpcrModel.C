// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
//

#include <BALL/QSAR/kpcrModel.h>
#include <BALL/QSAR/pcrModel.h>
#include <newmatio.h>


using namespace BALL::QSAR;


KPCRModel::KPCRModel(const QSARData& q, int k_type, double p1, double p2) : KernelModel(q,k_type,p1,p2) 
{
	type_="KPCR";
	frac_var_=0.99;
}

KPCRModel::KPCRModel(const QSARData& q, RowVector& w) : KernelModel(q, w) 
{
	type_="KPCR";
	frac_var_=0.99;
}

KPCRModel::KPCRModel(const QSARData& q, String s1, String s2) : KernelModel(q,s1,s2) 
{
	type_="KPCR";
	frac_var_=0.99;
}


KPCRModel::KPCRModel(const QSARData& q, const LinearModel& lm, int column) : KernelModel(q,lm,column) 
{
	type_="KPCR";
	frac_var_=0.99;
}


KPCRModel::~KPCRModel()
{
}
	

void KPCRModel::setFracVar(double frac_var)
{
	frac_var_=frac_var;
}


void KPCRModel::train()
{
	if(descriptor_matrix_.Ncols()==0)
	{
		throw Exception::InconsistentUsage(__FILE__,__LINE__,"Data must be read into the model before training!");
	}
	
	kernel->calculateKernelMatrix(descriptor_matrix_, K_);
	
	SymmetricMatrix S(descriptor_matrix_.Nrows());
	S << K_;
	PCRModel::calculateEigenvectors(S,frac_var_,loadings_);

	latent_variables_.ReSize(descriptor_matrix_.Nrows(),loadings_.Ncols());
	for(uint i=1;i<=loadings_.Ncols();i++)
	{
		latent_variables_.Column(i)=K_*loadings_.Column(i);
	}

	RRModel m(*data);
	m.descriptor_matrix_=latent_variables_;
	m.Y_=Y_;
	m.train();
	
	//result of RR is a linear combination of latente variables 
	// = column with length=no of latente variables => matrix for more than one modelled activity
	weights_ = *m.getTrainingResult();
	training_result_ = loadings_*weights_;
}


RowVector KPCRModel::predict(const vector<double>& substance, bool transform)
{
	if(training_result_.Ncols()==0)
	{
		throw Exception::InconsistentUsage(__FILE__,__LINE__,"Model must be trained before it can predict the activitiy of substances!");
	}
	RowVector input=getSubstanceVector(substance,transform);
	Matrix K_t(input.Nrows(), descriptor_matrix_.Nrows());
	kernel->calculateKernelMatrix(K_, input, descriptor_matrix_, K_t); // dim: 1xn
	RowVector res = K_t*training_result_; // dim: 1xc
	
	if(transform && y_transformations_.Ncols()!=0)
	{
		backTransformPrediction(res);
	}
	return res;	
}


void KPCRModel::setParameters(vector<double>& v)
{
	if(v.size()!=1)
	{
		String c = "Wrong number of model parameters! Needed: 1;";
		c = c+" given: "+String(v.size());
		throw Exception::ModelParameterError(__FILE__,__LINE__,c.c_str());
	}
	frac_var_ = v[0];
}


vector<double> KPCRModel::getParameters() const
{
	vector<double> d;
	d.push_back(frac_var_);
	return d;
}
