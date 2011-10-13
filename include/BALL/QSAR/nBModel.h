/* nBModel.h
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

#ifndef NBMODEL
#define NBMODEL

#ifndef BALL_QSAR_COMMON_H
#include <BALL/QSAR/common.h>
#endif

#ifndef BAYESMODEL
#include <BALL/QSAR/bayesModel.h>
#endif

#ifndef STATISTICS
#include <BALL/QSAR/statistics.h>
#endif

namespace BALL
{
	namespace QSAR 
	{

		/** class for Naive Bayes */
		class BALL_EXPORT NBModel : public BayesModel
		{ 
			public:
				/** @name Constructors and Destructors
				 */
				//@{
				NBModel(const QSARData& q);

				~NBModel();

				EIGEN_MAKE_ALIGNED_OPERATOR_NEW
				//@}
				
				
				/** @name Accessors
				 */
				//@{
				void train();
				
				Eigen::VectorXd predict(const vector<double>& substance, bool transform=1);
				
				void saveToFile(string filename);
				
				void readFromFile(string filename);
				
				vector<double> getParameters() const;
				
				void setParameters(vector<double>& v);
				
				bool isTrained();
				
				vector<double> calculateProbabilities(int activitiy_index, int feature_index, double feature_value);
				
				int getNoResponseVariables();	
				//@}
				
				
			private:
				/** @name Attributes
				 */
				//@{
				
				uint discretization_steps_;
				
				
				/** the minmum (row1) and maximum (row2) of each feature. */
				Eigen::MatrixXd min_max_;
				
				/** One probability Matrix for each modelled activity and each class. \n
				Each Matrix stores in each cell the probability for a feature lying within a specific range to be in a specific class  */
				vector<MatrixVector, Eigen::aligned_allocator<MatrixVector> > probabilities_;
							

				
				//@}
		};
	}
}

#endif //NBMODEL
