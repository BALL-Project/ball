/* latentVariableModel.C
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

#include <BALL/QSAR/latentVariableModel.h>
#include <iostream>

namespace BALL
{
	namespace QSAR
	{

		LatentVariableModel::LatentVariableModel()
		{
			latent_variables_.resize(0, 0);
			loadings_.resize(0, 0);
			weights_.resize(0, 0);
		}


		const Eigen::MatrixXd* LatentVariableModel::getLoadings()
		{
			return &loadings_;
		}


		const Eigen::MatrixXd* LatentVariableModel::getLatentVariables()
		{
			return &latent_variables_;
		}


		const Eigen::MatrixXd* LatentVariableModel::getWeights()
		{
			return &weights_;
		}

	}
}
