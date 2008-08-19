  // -*- Mode: weights_++; tab-width: 2; -*-
// vi: set ts=2:
//
//

#include <BALL/QSAR/plsModel.h>
#include <newmatio.h>

//using namespace std;
using namespace BALL::QSAR;



PLSModel::PLSModel(const QSARData& q) : LinearModel(q) 
{
	type_="PLS";
	no_components_=10;
}

PLSModel::~PLSModel()
{
}

int PLSModel::getNoComponents()
{
	return no_components_;
}

void PLSModel::setNoComponents(int no)
{
	no_components_=no;
}
	

void PLSModel::train()
{	
	if(descriptor_matrix_.Ncols()==0)
	{
		throw Exception::InconsistentUsage(__FILE__,__LINE__,"Data must be read into the model before training!");
	}
	int cols=descriptor_matrix_.Ncols();

	Matrix X=descriptor_matrix_;
	Matrix P;  // Matrix P saves all vectors p

	ColumnVector w;
	ColumnVector t;
	ColumnVector c;
	ColumnVector u(X.Nrows());
	for (int i=1; i<=u.Nrows(); i++)
	{
		u(i)=Y_(i,1);	
	}
	ColumnVector u_old;
	
	for(int j=0; j<no_components_ && j<cols; j++)
	{
		for(int i=0; i<10000;i++)
		{	
			w = X.t()*u / Statistics::scalarProduct(u);
			w = w / Statistics::euclNorm(w);
			t = X*w;
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
		Matrix p = X.t()*t / Statistics::scalarProduct(t);
		X = X - t*p.t();
		//Y_ = Y_ - t*c.t();
		
		if(j==0)
		{
			U_ = u;
			loadings_ = w;	
			weights_ = c;
			P = p;
			latent_variables_ = t;
		}
		else
		{
			//w = w*(p.t()*w).i();
			U_ = U_|u;
			loadings_ = loadings_|w;
			weights_ = weights_|c;
			P = P|p;
			latent_variables_ = latent_variables_ | t;
		}
	}

	try  // p's are not orthogonal to each other, so that in rare cases P.t()*W_ is not invertible
	{
		loadings_ = loadings_*(P.t()*loadings_).i();
	}
	catch(SingularException e)
	{
		IdentityMatrix I(P.Ncols());
		I = I*0.0001;
		loadings_ = loadings_*(P.t()*loadings_+I).i();
	}
	training_result_=loadings_*weights_.t();

}


bool PLSModel::optimizeParameters(int k, int no_steps)
{
	double best_q2=0;
	int best_no=1;
	for(unsigned int i=1; i<=no_steps && i<=data->getNoDescriptors() && (descriptor_IDs_.empty() || i<descriptor_IDs_.size()); i++)
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
			break; // for speed-up
		}
	}
	no_components_=best_no;
	validation->setQ2(best_q2);
	
	return 1;
}


const Matrix* PLSModel::getU()
{ 
	return &U_;
}


void PLSModel::setParameters(vector<double>& v)
{
	if(v.size()!=1)
	{
		String c = "Wrong number of model parameters! Needed: 1;";
		c = c+" given: "+String(v.size());
		throw Exception::ModelParameterError(__FILE__,__LINE__,c.c_str());
	}
	no_components_ = (int) v[0];
}

vector<double> PLSModel::getParameters() const
{	
	vector<double> v;
	v.push_back(no_components_);
	return v;
}
	
