  // -*- Mode: C_++; tab-width: 2; -*-
// vi: set ts=2:
//
//

#include <BALL/QSAR/plsModel.h>
#include <newmat/newmatio.h>

//using namespace std;
using namespace BALL::QSAR;



PLSModel::PLSModel(const QSARData& q) : LinearModel(q) 
{
	type_="PLS";
	no_components_=1;
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

bool PLSModel::optimizeParameters(int k)
{
	findNoComponents(k);
	return 1;
}
	

void PLSModel::train()
{	
	if(descriptor_matrix_.Ncols()==0)
	{
		throw Exception::InconsistentUsage(__FILE__,__LINE__,"Data must be read into the model before training!");
	}
	int cols=descriptor_matrix_.Ncols();

	Matrix X=descriptor_matrix_;
	//Matrix U_; // Matrix U_ saves all vectors u
	//Matrix W_; // Matrix W_ saves all vectors w
	//Matrix C_; // Matrix C_ saves all vectors c
	//Matrix T_;  // Matrix T_ saves all vectors t
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
			W_ = w;	
			C_ = c;
			P = p;
			T_ = t;
		}
		else
		{
			//w = w*(p.t()*w).i();
			U_ = U_|u;
			W_ = W_|w;
			C_ = C_|c;
			P = P|p;
			T_ = T_ | t;
		}
	}

	try  // p's are not orthogonal to each other, so that in rare cases P.t()*W_ is not invertible
	{
		W_ = W_*(P.t()*W_).i();
	}
	catch(SingularException e)
	{
		IdentityMatrix I(P.Ncols());
		I = I*0.0001;
		W_ = W_*(P.t()*W_+I).i();
	}
	training_result_=W_*C_.t();

}



void PLSModel::findNoComponents(int k)
{
	double best_q2=0;
	int best_no=1;
	for(unsigned int i=1; i<=data->getNoDescriptors() && (descriptor_IDs_.empty() || i<descriptor_IDs_.size()) ;i++)
	{
		no_components_=i;
		validation->crossValidation(k);

		if(validation->getQ2()>best_q2)
		{
			best_q2=validation->getQ2();
			best_no=i;
		}
// 		else
// 		{
// 			break;
// 		}
	}
	no_components_=best_no;
	validation->setQ2(best_q2);
}


const Matrix* PLSModel::getT()
{ 
	return &T_;
}


const Matrix* PLSModel::getW()
{
	return &W_;
}

const Matrix* PLSModel::getU()
{ 
	return &U_;
}


const Matrix* PLSModel::getC()
{
	return &C_;
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
	
