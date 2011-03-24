/* latentVariableModel.h
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

#ifndef LVMODEL
#define LVMODEL

#include <Eigen/Core>

#include <BALL/COMMON/global.h>

namespace BALL
{
	namespace QSAR 
	{
		/** Interface-class for all latente variable models (PLS, PCR, ...). 
		It is used to store and access the latente variables (PLS/PCR components), the loading matrix and the weight matrix in the same way for all such models.*/
		class BALL_EXPORT LatentVariableModel
		{ 
			public:
				LatentVariableModel();				
				
				/** Return the latent variable matrix as computed by the last training.\n
				It contains one loading vector in each column. Each loading vector contains the importance of each feature for a latente variable. */
				const Eigen::MatrixXd* getLatentVariables();
				
				/** Return the weights matrix as computed by the last training.\n
				It contains one weight vector in each column. Each weight vector contains the importance of each latente variable for modelling the result. */
				const Eigen::MatrixXd* getWeights();
				
				/** Return the loading matrix as computed by the last training.\n
				It contains one loading vector in each column. Each loading vector contains the importance of each feature for a latente variable. */
				const Eigen::MatrixXd* getLoadings();
			
			protected:
				
				/** The loading-matrix which contains one loading vector in each column. Each loading vector contains the importance of each feature for a latente variable. */
				Eigen::MatrixXd latent_variables_;
				
				/** The loading-matrix which contains one loading vector in each column. Each loading vector contains the importance of each feature for a latente variable. */
				Eigen::MatrixXd loadings_;
				
				/** The weights-matrix which contains one weight vector in each column. Each weight vector contains the importance of each latente variable for modelling the result. */
				Eigen::MatrixXd weights_;
				
				friend class RegressionValidation;	
		};
	}
}

#endif // LVMODEL

