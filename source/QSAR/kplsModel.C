// -*- Mode: weights_++; tab-width: 2; -*-
// vi: set ts=2:
//
//

#include <BALL/QSAR/kplsModel.h>
#include <newmatio.h>
#include <BALL/QSAR/mlrModel.h>

using namespace BALL::QSAR;


KPLSModel::KPLSModel(const QSARData& q, int k_type, double p1, double p2) : KernelModel(q,k_type,p1,p2) 
{
	no_components_=1;
	type_="KPLS";
}


KPLSModel::KPLSModel(const QSARData& q, RowVector& w) : KernelModel(q,w) 
{
	no_components_=1;
	type_="KPLS";
}


KPLSModel::KPLSModel(const QSARData& q, String s1, String s2) : KernelModel(q,s1,s2) 
{
	no_components_=1;
	type_="KPLS";
}


KPLSModel::KPLSModel(const QSARData& q, const LinearModel& lm, int column) : KernelModel(q,lm,column)
{
	no_components_=1;
	type_="KPLS";
}


KPLSModel::~KPLSModel()
{
}

int KPLSModel::getNoComponents()
{
	return no_components_;
}

void KPLSModel::setNoComponents(int d)
{
	no_components_=d;
}


void KPLSModel::setParameters(vector<double>& v)
{
	if(v.size()!=1)
	{
		String c = "Wrong number of model parameters! Needed: 1;";
		c = c+" given: "+String(v.size());
		throw Exception::ModelParameterError(__FILE__,__LINE__,c.c_str());
	}
	no_components_ = (int) v[0];
}


vector<double> KPLSModel::getParameters() const
{
	vector<double> d;
	d.push_back(no_components_);
	return d;
}


void KPLSModel::train()
{	
	if(descriptor_matrix_.Ncols()==0)
	{
		throw Exception::InconsistentUsage(__FILE__,__LINE__,"Data must be read into the model before training!");
	}
// 	if(descriptor_matrix_.Ncols()<no_components_)
// 	{
// 		throw Exception::TooManyPLSComponents(__FILE__,__LINE__,no_components_,descriptor_matrix_.Ncols());
// 	}
	
	kernel->calculateKernelMatrix(descriptor_matrix_, K_);

	//Matrix U_; // Matrix U_ saves all vectors u
	//Matrix loadings_; // Matrix loadings_ saves all vectors w
	//Matrix weights_; // Matrix weights_ saves all vectors c
	//Matrix latent_variables_;  // Matrix latent_variables_ saves all vectors t
	Matrix P;  // Matrix P saves all vectors p

	ColumnVector w;
	ColumnVector t;
	ColumnVector c;
	ColumnVector u(K_.Nrows());
	for (int i=1; i<=u.Nrows(); i++)
	{
		u(i)=Y_(i,1);	
	}
	ColumnVector u_old;
	int cols = descriptor_matrix_.Ncols();
	
	for(int j=0; j<no_components_ & j<cols; j++)
	{
		for(int i=0;i<10000 ;i++)
		{	
			w = K_.t()*u / Statistics::scalarProduct(u);
			w = w / Statistics::euclNorm(w);
			t = K_*w;
			c = Y_.t()*t / Statistics::scalarProduct(t);
			u_old=u;
			u = Y_*c / Statistics::scalarProduct(c); 
		
			if (Statistics::euclDistance(u,u_old)/Statistics::euclNorm(u)>0.0000001) 
			{ 
				continue;				
			}
			else  // if u has converged
			{
				break;
			}
		}
		Matrix p = K_.t()*t / Statistics::scalarProduct(t);
		K_ = K_ - t*p.t();
		//Y_ = Y_ - t*c.t();
		
		if(j==0)
		{
			loadings_ = w;	
			weights_ = c;
			P = p;
			latent_variables_ = t;
		}
		else
		{
			loadings_ = loadings_|w;
			weights_ = weights_|c;
			P = P|p;
			latent_variables_ = latent_variables_ | t;
		}
	}

	loadings_ = loadings_*(P.t()*loadings_).i();
	training_result_=loadings_*weights_.t();
	
	calculateOffsets();
}


bool KPLSModel::optimizeParameters(int k, int no_steps)
{
	double best_q2=0;
	int best_no=1;
	for(unsigned int i=1; i<=no_steps && i<=data->getNoDescriptors() && (descriptor_IDs_.empty() || i<descriptor_IDs_.size());i++)
	{	
		no_components_=i;
		validation->crossValidation(k);
		if(validation->getQ2()>best_q2)
		{
			best_q2=validation->getQ2();
			best_no=i;
		}
		else if(validation->getQ2()<0.75*best_q2)
		{
			break;
		}
	}
	no_components_=best_no;
	validation->setQ2(best_q2);
	
	return 1;
}

const Matrix* KPLSModel::getU()
{ 
	return &U_;
}
