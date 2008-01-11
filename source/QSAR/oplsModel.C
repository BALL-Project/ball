 // -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
//

#include <BALL/QSAR/oplsModel.h>
#include <newmat/newmatio.h>
#include <BALL/QSAR/statistics.h>
 
using namespace BALL::QSAR;


OPLSModel::OPLSModel(const QSARData& q) : PLSModel(q) 
{ 
	type_="OPLS";
	no_ortho_components_=20;
}

OPLSModel::~OPLSModel()
{
}

void OPLSModel::train()
{
	Matrix X=descriptor_matrix_;
	
	
// 	double d[]={-1,-1,1,-1,-1,1,1,1};
// 	//double d[]={-2.18,-2.18,1.84,-0.16,-0.48,1.52,0.83,0.83};
// 	Matrix X(4,2);
// 	X << d;
// 	Y_.ReSize(4,1);
// 	double e[]={2,2,0,-4};
// 	Y_ << e;
	int cols = descriptor_matrix_.Ncols();
	
	ColumnVector w;
	ColumnVector t;
	ColumnVector c;
	ColumnVector u(X.Nrows());
	
	for (int i=1; i<=u.Nrows(); i++)
	{
		u(i)=Y_(i,1);
	}
	ColumnVector u_old;
	//w = X.t()*u / Statistics::scalarProduct(u);
	//w = w / Statistics::euclNorm(w);	

	for(int j=0; j<no_ortho_components_ && j<cols; j++)
	{	
		for(int i=0; ;i++)
		{
			w = X.t()*u / Statistics::scalarProduct(u);
			w = w / Statistics::euclNorm(w);	
			
			t = X*w ;
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
	
		ColumnVector p = X.t()*t / Statistics::scalarProduct(t);
		ColumnVector w_ortho = p - ((w.t()*p).AsScalar() / (w.t()*w).AsScalar())*w;
		w_ortho = w_ortho / Statistics::euclNorm(w_ortho);
		ColumnVector t_ortho = X*w_ortho / Statistics::scalarProduct(w_ortho); 
		ColumnVector p_ortho = X.t()*t_ortho / Statistics::scalarProduct(t_ortho);
		
		Matrix E_OPLS = X - t_ortho*p_ortho.t(); 
		X = E_OPLS;
		
		if(j==0)
		{
			W_ortho_ = w_ortho;
			//W = w;	
			//C = c;
			//P = p;
			//P_ortho = p_ortho;
			T_ortho_ = t_ortho;
		}
		else
		{
			W_ortho_ = W_ortho_ | w_ortho;
			//W = W|w;
			//C = C|c;
			//P = P|p;
			//P_ortho = P_ortho | p_ortho;
			T_ortho_ = T_ortho_ | t_ortho;
		}
	}
		
	descriptor_matrix_ = X;
	PLSModel::train();
}


const Matrix* OPLSModel::getTOrtho()
{ 
	return &T_ortho_;
}


const Matrix* OPLSModel::getWOrtho()
{
	return &W_ortho_;
}


void OPLSModel::optimizeParameters(int k, unsigned int max)
{
	double best_q2=0;
	int best_no=1;
	int best_o_no=1;
	int cols = data->getNoDescriptors();
	if(!descriptor_IDs_.empty())
	{
		cols = descriptor_IDs_.size();
	}
	
	for(unsigned int i=1; i<=max && i<=cols;i++)
	{	
		no_ortho_components_=i;
		for(unsigned int j=1; j<=max && j<=cols;j++)
		{
			no_components_=j;
			validation->crossValidation(k);
			if(validation->getQ2()>best_q2)
			{
				best_q2=validation->getQ2();
				best_no=j;
				best_o_no=i;
			}
			else if(validation->getQ2()<0.5*best_q2)
			{
				break;
			}
		}
	}
	no_components_=best_no;
	no_ortho_components_=best_o_no;
	validation->setQ2(best_q2);
}


int OPLSModel::getNoOrthoComponents()
{
	return no_ortho_components_;
}
				
				
void OPLSModel::setNoOrthoComponents(int d)
{
	no_ortho_components_=d;
}


void OPLSModel::setParameters(vector<double>& v)
{
	if(v.size()!=2)
	{
		String c = "Wrong number of model parameters! Needed: 2;";
		c = c+" given: "+String(v.size());
		throw Exception::ModelParameterError(__FILE__,__LINE__,c.c_str());
	}
	no_ortho_components_ = (int)v[0];
	no_components_ = (int)v[1];
}


vector<double> OPLSModel::getParameters() const
{
	vector<double> d;
	d.push_back(no_ortho_components_);
	d.push_back(no_components_);
	return d;
}

// RowVector OPLSModel::predict(const vector<double>& substance, bool transform)
// {
// 	if(training_result_.Ncols()==0)
// 	{
// 		throw Exception::InconsistentUsage(__FILE__,__LINE__,"Model must be trained before it can predict the activitiy of substances!");
// 	}
// 
// 	RowVector v=getSubstanceVector(substance, transform);
// 	
// // 	ColumnVector t_new_ortho = v*W_ortho_.Column(1) / (W_ortho_.Column(1).t()*W_ortho_.Column(1)).AsScalar();
// // 	v = v - t_new_ortho*P_ortho.Column(1).t();
// 	
// 	RowVector res=v*training_result_;
// 
// 	if(transform && y_transformations_.Ncols()!=0)
// 	{
// 		backTransformPrediction(res);
// 	}
// 
// 	return res;
// }

