/* kpcrModel.C
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

#include <BALL/QSAR/kpcrModel.h>
#include <BALL/QSAR/pcrModel.h>
#include <BALL/MATHS/LINALG/matrixInverter.h>

namespace BALL
{
	namespace QSAR
			{


		KPCRModel::KPCRModel(const QSARData& q, int k_type, double p1, double p2) : KernelModel(q, k_type, p1, p2) 
		{
			type_="KPCR";
			frac_var_ = 0.99;
		}

		KPCRModel::KPCRModel(const QSARData& q, Vector<double>& w) : KernelModel(q, w) 
		{
			type_="KPCR";
			frac_var_ = 0.99;
		}

		KPCRModel::KPCRModel(const QSARData& q, String s1, String s2) : KernelModel(q, s1, s2) 
		{
			type_="KPCR";
			frac_var_ = 0.99;
		}


		KPCRModel::KPCRModel(const QSARData& q, const LinearModel& lm, int column) : KernelModel(q, lm, column) 
		{
			type_="KPCR";
			frac_var_ = 0.99;
		}


		KPCRModel::~KPCRModel()
		{
		}
			

		void KPCRModel::setFracVar(double frac_var)
		{
			frac_var_ = frac_var;
		}


		void KPCRModel::train()
		{
			if (descriptor_matrix_.Ncols() == 0)
			{
				throw Exception::InconsistentUsage(__FILE__, __LINE__, "Data must be read into the model before training!"); 
			}
			
			kernel->calculateKernelMatrix(descriptor_matrix_, K_);
			
			PCRModel::calculateEigenvectors(K_, frac_var_, loadings_);
			
			latent_variables_ = K_*loadings_;

			//result of RR is a linear combination of latente variables 
			// = column with length = no of latente variables = > matrix for more than one modelled activity
			Matrix<double> m = latent_variables_.t()*latent_variables_;
			MatrixInverter<double, StandardTraits> inverter(m);
			inverter.computePseudoInverse();

			weights_ = inverter.getPseudoInverse()*latent_variables_.t()*Y_;
			training_result_ = loadings_*weights_;
			
			calculateOffsets();
		}


		void KPCRModel::setParameters(vector<double>& v)
		{
			if (v.size() != 1)
			{
				String c = "Wrong number of model parameters! Needed: 1;";
				c = c+" given: "+String(v.size());
				throw Exception::ModelParameterError(__FILE__, __LINE__, c.c_str());
			}
			frac_var_ = v[0];
		}


		vector<double> KPCRModel::getParameters() const
		{
			vector<double> d;
			d.push_back(frac_var_);
			return d;
		}
	}
}
