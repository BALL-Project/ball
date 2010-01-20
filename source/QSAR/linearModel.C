/* linearModel.C
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

#include <BALL/QSAR/linearModel.h>
//#include <newmatio.h>

using namespace BALL::QSAR;



LinearModel::LinearModel(const QSARData& q) : RegressionModel(q) {}

LinearModel::~LinearModel()
{
}

void LinearModel::operator=(const Model& m)
{
	Model::operator=(m);
	validation->setCoefficientStdErrors(((LinearModel*) &m)->validation->getCoefficientStdErrors());
}


void LinearModel::calculateOffsets()
{
	Matrix<double> residuals = (descriptor_matrix_*training_result_)-Y_;
	int no_act=training_result_.Ncols();
	offsets_.resize(no_act);
	offsets_.setVectorType(0); // row-vector
	for(int i=1; i<=no_act; i++)
	{	
		offsets_(i) = residuals.colSum(i) / training_result_.Nrows();
	}
	//cout<<"offset : "<<offsets_(1)<<endl<<flush;
}


BALL::Vector<double> LinearModel::predict(const vector<double>& substance, bool transform)
{
	if(training_result_.getSize()==0)
	{
		throw Exception::InconsistentUsage(__FILE__,__LINE__,"Model must be trained before it can predict the activitiy of substances!");
	}

	Vector<double> v=getSubstanceVector(substance, transform);

	Vector<double> res=v*training_result_;
	//if(offsets_.getSize()==res.getSize()) res -= offsets_;

	if(transform && y_transformations_.Ncols()!=0)
	{
		backTransformPrediction(res);
	}
	
	return res;
}
