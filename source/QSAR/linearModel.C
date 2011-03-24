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

namespace BALL
{
	namespace QSAR
	{

		LinearModel::LinearModel(const QSARData& q) : RegressionModel(q) {}

		LinearModel::~LinearModel()
		{
		}

		void LinearModel::operator = (const LinearModel& m)
		{
			RegressionModel::operator = (m);
			validation->setCoefficientStdErrors(m.validation->getCoefficientStdErrors());
		}


		void LinearModel::calculateOffsets()
		{
			offsets_ = ((descriptor_matrix_*training_result_)-Y_).colwise().sum() / training_result_.rows();
			//cout<<"offset : "<<offsets_(1)<<endl<<flush;
		}


		Eigen::VectorXd LinearModel::predict(const vector<double> & substance, bool transform)
		{
			if (training_result_.rows() == 0)
			{
				throw Exception::InconsistentUsage(__FILE__, __LINE__, "Model must be trained before it can predict the activitiy of substances!"); 
			}

			Eigen::VectorXd v = getSubstanceVector(substance, transform); 

			Eigen::VectorXd res = v.transpose()*training_result_;
			//if (offsets_.getSize() == res.getSize()) res -= offsets_; 

			if (transform && y_transformations_.cols() != 0)
			{
				backTransformPrediction(res); 
			}
			
			return res;
		}
	}
}
