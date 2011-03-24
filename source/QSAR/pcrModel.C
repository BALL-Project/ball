/* pcrModel.C
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

#include <BALL/QSAR/pcrModel.h>
#include <Eigen/Dense>

namespace BALL
{
	namespace QSAR
			{

		PCRModel::PCRModel(const QSARData& q, double frac_var) : LinearModel(q) 
		{
			type_="PCR";
			frac_var_ = frac_var;
		}

		PCRModel::~PCRModel()
		{
		}

		void PCRModel::setFracVar(double frac_var)
		{
			frac_var_ = frac_var;
		}

		void PCRModel::calculateEigenvectors(const Eigen::MatrixXd& data, double frac_var, Eigen::MatrixXd& output)
		{
			Eigen::JacobiSVD<Eigen::MatrixXd> svd;
			svd.compute(data, Eigen::ComputeThinV);

			// find the smallest singular vector that should be taken into account
			// complete variance == sum of all eigen-values == sum of squared singular values
			double complete_var = svd.singularValues().sum();

			double explained_var = 0;
			uint cols = 0;
			if (complete_var < 5*std::numeric_limits < double > ::epsilon())
			{
				throw Exception::NoPCAVariance(__FILE__, __LINE__, "No variance present to be explained by PCA!");
			}

			uint last_vector = 0;
			for (; last_vector < svd.singularValues().rows() && cols < data.rows() && explained_var/complete_var < frac_var ; last_vector++)
			{
				// (singular-value)^2 == eigen-value
				explained_var += svd.singularValues()[last_vector];
				cols++;
			}

			output.resize(data.rows(), cols);

			for (uint i = 0; i < data.rows(); i++)
			{
				for (uint j = 0; j < last_vector ; j++)
				{
					output(i, j) = svd.matrixV()(i, j);
				}
			}
		}


		void PCRModel::train()
		{
			if (descriptor_matrix_.cols() == 0)
			{
				throw Exception::InconsistentUsage(__FILE__, __LINE__, "Data must be read into the model before training!"); 
			}
			Eigen::MatrixXd XX = descriptor_matrix_.transpose()*descriptor_matrix_;
			
			calculateEigenvectors(XX, frac_var_, loadings_);

			latent_variables_ = descriptor_matrix_*loadings_;
			
			RRModel m(*data);
			m.descriptor_matrix_ = latent_variables_;
			m.Y_ = Y_;
			m.train();

			//result of RR is a linear combination of latente variables 
			// = column with length = no of latente variables = > matrix for more than one modelled activity
			weights_ = *m.getTrainingResult();
			training_result_ = loadings_*weights_;
			
			calculateOffsets();
		}


		void PCRModel::setParameters(vector<double>& v)
		{
			if (v.size() != 1)
			{
				String c = "Wrong number of model parameters! Needed: 1;";
				c = c+" given: "+String(v.size());
				throw Exception::ModelParameterError(__FILE__, __LINE__, c.c_str());
			}
			frac_var_ = v[0];
		}


		vector<double> PCRModel::getParameters() const
		{
			vector<double> d;
			d.push_back(frac_var_);
			return d;
		}
	}
}
