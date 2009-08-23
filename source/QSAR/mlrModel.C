/* mlrModel.C
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

#include <BALL/QSAR/mlrModel.h>
using namespace BALL::QSAR;


MLRModel::MLRModel(const QSARData& q) : LinearModel(q) 
{
	type_="MLR";
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
	if (descriptor_matrix_.Ncols()>=descriptor_matrix_.Nrows())
	{	
		throw Exception::SingularMatrixError(__FILE__,__LINE__,"For MLR model, matrix must have more rows than columns in order to be invertible!!");
		//training_result_.ReSize(0,0);
		//return;
	}

  	Matrix<double> m = descriptor_matrix_.t()*descriptor_matrix_;

		
	try
	{
		training_result_ = m.i()*descriptor_matrix_.t()*Y_;
	}
	catch(BALL::Exception::GeneralException e)
	{
		training_result_.ReSize(0,0);
		throw Exception::SingularMatrixError(__FILE__,__LINE__,"Matrix for MLR training is singular!! Check that descriptor_matrix_ does not contain empty columns!");
		return;
	}

	calculateOffsets();
}
