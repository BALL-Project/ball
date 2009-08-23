/* oplsModel.C
 * 
 * Copyright (C) 2009 Marcel Schumann
 * 
 * This file is part of QuEasy -- A Toolbox for Automated QSAR Model
 * Construction and Validation.
 * QuEasy is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or (at
 * your option) any later version.
 * 
 * QuEasy is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, see <http://www.gnu.org/licenses/>.
 */

 // -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
//

#include <BALL/QSAR/oplsModel.h>
#include <BALL/QSAR/statistics.h>
 
using namespace BALL::QSAR;


OPLSModel::OPLSModel(const QSARData& q) : PLSModel(q) 
{ 
	type_="OPLS";
	no_ortho_components_=5;
}

OPLSModel::~OPLSModel()
{
}

void OPLSModel::train()
{
	Matrix<double> X=descriptor_matrix_;
	
	
// 	double d[]={-1,-1,1,-1,-1,1,1,1};
// 	//double d[]={-2.18,-2.18,1.84,-0.16,-0.48,1.52,0.83,0.83};
// 	Matrix X(4,2);
// 	X << d;
// 	Y_.ReSize(4,1);
// 	double e[]={2,2,0,-4};
// 	Y_ << e;
	int cols = descriptor_matrix_.Ncols();
	
	Vector<double> w; w.setVectorType(1); // column-vector
	Vector<double> t; t.setVectorType(1); // column-vector
	Vector<double> c; c.setVectorType(1); // column-vector
	Vector<double> u(X.Nrows());  u.setVectorType(1); // column-vector
	
	for (uint i=1; i<=u.getSize(); i++)
	{
		u(i)=Y_(i,1);
	}
	Vector<double> u_old; u_old.setVectorType(1); // column-vector
	//w = X.t()*u / Statistics::scalarProduct(u);
	//w = w / Statistics::euclNorm(w);	
	
	// determine the number of orthogonal components that are to be created.
	// no_ortho_components_ contains the number of components desired by the user, 
	// but obviously the number of orthogonal components must be <= #features-1
	// since we will need at least one non-orthogonal component as well
	uint orthogonal_components_to_create = no_ortho_components_;
	if(cols-1<no_ortho_components_) orthogonal_components_to_create=cols-1;
	
	W_ortho_.resize(cols,orthogonal_components_to_create);
	T_ortho_.resize(descriptor_matrix_.Nrows(),orthogonal_components_to_create);

	for(uint j=0; j<orthogonal_components_to_create; j++)
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
	
		Vector<double> p = X.t()*t / Statistics::scalarProduct(t);
		Vector<double> w_ortho = w*((w.t()*p) / (w.t()*w));
		w_ortho = p-w_ortho;
		w_ortho = w_ortho / Statistics::euclNorm(w_ortho);
		Vector<double> t_ortho = X*w_ortho / Statistics::scalarProduct(w_ortho); 
		Vector<double> p_ortho = X.t()*t_ortho / Statistics::scalarProduct(t_ortho);
		
		Matrix<double> TP; 
		t_ortho.dotProduct(p_ortho,TP); // t.p.t() -> dim. nxm
		X -= TP; 
		
		W_ortho_.copyVectorToColumn(w_ortho,j+1);
		T_ortho_.copyVectorToColumn(t_ortho,j+1);
	}
		
	descriptor_matrix_ = X;
	PLSModel::train();
}


const BALL::Matrix<double>* OPLSModel::getTOrtho()
{ 
	return &T_ortho_;
}


const BALL::Matrix<double>* OPLSModel::getWOrtho()
{
	return &W_ortho_;
}


bool OPLSModel::optimizeParameters(int k, int no_steps)
{
	double best_q2=0;
	int best_no=1;
	int best_o_no=1;
	int cols = data->getNoDescriptors();
	if(!descriptor_IDs_.empty())
	{
		cols = descriptor_IDs_.size();
	}
	no_ortho_components_=0;
	
	// first find best number of components, for PLS analysis 
	for(int i=1; i<=no_steps && i<=cols;i++)
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
	best_q2=validation->getQ2();
	
	//then try to remove orthogonal variance by use of OPLS components
	for(int i=0; i<=no_steps && i<=cols;i++)
	{	
		no_ortho_components_=i;
			
		validation->crossValidation(k);
	
		if(validation->getQ2()>best_q2)
		{
			best_q2=validation->getQ2();
			best_o_no=i;
		}
		else if(validation->getQ2()<0.75*best_q2)
		{
			break; // for speed-up
		}
	}
	no_ortho_components_=best_o_no;
	best_q2=validation->getQ2();

	//finally, try to reduce the number of PLS components (since orthogonal variance has been removed, less PLS components should be neccessary)
	for(int i=no_components_; i>=1 ;i--)
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

