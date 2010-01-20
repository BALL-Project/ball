/* rrModel.C
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

#include <BALL/QSAR/rrModel.h>
//#include <BALL/SYSTEM/timer.h>

using namespace BALL::QSAR;


RRModel::RRModel(const QSARData& q, double lambda) : MLRModel(q) 
{
	type_="RR";
	lambda_=lambda;
}

RRModel::~RRModel()
{
}


void RRModel::train()
{	
	if(descriptor_matrix_.Ncols()==0)
	{
		throw Exception::InconsistentUsage(__FILE__,__LINE__,"Data must be read into the model before training!");
	}
	if(Y_.Ncols()==0)
	{
		throw Exception::InconsistentUsage(__FILE__,__LINE__,"No response values have been read! Model can not be trained!");
	}
	if (lambda_==0 && descriptor_matrix_.Ncols()>=descriptor_matrix_.Nrows())
	{	
		throw Exception::SingularMatrixError(__FILE__,__LINE__,"For MLR model, matrix must have more rows than columns in order to be invertible!!");
		//training_result_.ReSize(0,0);
		//return;
	}

  	Matrix<double> m = descriptor_matrix_.t()*descriptor_matrix_;

	if(lambda_ != 0)
	{
		Matrix<double> I; I.setToIdentity(m.Nrows());
		I*=lambda_;
		m+=I;
	}
		
	try
	{
	//	Timer timer; timer.start();
		training_result_ = m.i()*descriptor_matrix_.t()*Y_;
	//	timer.stop(); cout<<timer.getClockTime()<<endl;
		
// 		timer.start();
// 		Matrix X1 = m;
// 		Matrix Y1 = descriptor_matrix_.t()*Y_;
// 		UpperTriangularMatrix U; Matrix M;
// 		QRZ(X1, U); QRZ(X1, Y1, M);    // Y1 now contains resids
// 		Matrix test = U.i() * M;	
// 		timer.stop(); cout<<timer.getClockTime()<<endl;
// 		
// 		cout<<(training_result_-test).t()<<endl;
	}
	catch(BALL::Exception::GeneralException e)
	{
		training_result_.ReSize(0,0);
		throw Exception::SingularMatrixError(__FILE__,__LINE__,"Matrix for RR training is singular!! Check that descriptor_matrix_ does not contain empty columns and that lambda is not too small!");
		return;
	}
	
	calculateOffsets();	
}

void RRModel::setParameters(vector<double>& v)
{	
	if(v.size()!=1)
	{
		String c = "Wrong number of model parameters! Needed: 1;";
		c = c+" given: "+String(v.size());
		throw Exception::ModelParameterError(__FILE__,__LINE__,c.c_str());
	}
	lambda_=v[0];
}

vector<double> RRModel::getParameters() const
{
	vector<double> d;
	d.push_back(lambda_);
	return d;
}
