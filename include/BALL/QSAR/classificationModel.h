/* classificationModel.h
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

#ifndef CLASSIFICATION
#define CLASSIFICATION

#ifndef MODELH
#include <BALL/QSAR/Model.h>
#endif

#ifndef CLASVALIDATION
#include <BALL/QSAR/classificationValidation.h>
#endif

#ifndef QSAR_EXCEPTION
#include <BALL/QSAR/exception.h>
#endif


namespace BALL
{
	namespace QSAR
	{
		class BALL_EXPORT ClassificationModel : public Model
		{
			public:
				/** @name Constructors and Destructors
				 */
				//@{
				/** constructur,
				@param q QSAR-wrapper object, from which the data for this model should be taken */
				ClassificationModel(const QSARData& q);
				
				~ClassificationModel();

				virtual void operator=(ClassificationModel& m);
				//@}
				
				
				/** @name Attributes
				 */
				//@{
				/** a ClassificationValidation object, that is used to validate this model and that will contain the results of the validations */
				ClassificationValidation* validation;
				
				std::vector<int> getClassLabels();
				//@}
				
				
			protected:
	
				/** @name Accessors
				 */
				//@{
				/** read all different class labels from matrix Y into ClassificationModel.labels */
				void readLabels();
				
				/** the number of substances in each class of the training data */
				std::vector<int> no_substances_;
				//@}
				
				/** @name Input and Output. The following methods can be used to implement the functions saveToFile() and readFromFile() in final classes derived from this base-class 
				 */
				//@{
				void readClassInformationFromFile(std::ifstream& input, int no_classes);
				void saveClassInformationToFile(std::ofstream& out);
				//@}
				
				
				
				/** @name Feature discretization function
				 */
				//@{
				/** Interface function-definition for discretization of features.\n
				Thus, various functions that do the actual discretization can be created as members of ClassificationModel, allowing switching of the discretization technique by only (re-)setting this function-pointer.
				@param bins the number of discrete values to be created for each features 
				@param discretization_information a Eigen::MatrixXd into which the information about the applied discretization should be saved (into one column per feature). \n
				This Eigen::MatrixXd can later be used to discretize test data according to the discretization of the training data. */
				void (ClassificationModel::* discretizeFeatures)(unsigned int bins, Eigen::MatrixXd& discretization_information);
				
				/** Discretizes test data according to the given discretization of the training data.
				@param discretization_information the discretization according to which the current descriptor_matrix should be discretized. */
				void (ClassificationModel::* discretizeTestDataFeatures)(Eigen::VectorXd& compound, unsigned int bins, const Eigen::MatrixXd& discretization_information);
				
				
				/** discretizes each feature in such a way that each created bin has an equal range of values */
				void equalSpaceDiscretization(unsigned int bins, Eigen::MatrixXd& discretization_information);
				
				void equalSpaceDiscretizationTestData(Eigen::VectorXd& compound, unsigned int bins, const Eigen::MatrixXd& discretization_information);
				
				
				
				
				//@}
				
	
				/** @name Attributes
				 */
				//@{
				/** labels for the different classes */
				std::vector<int> labels_;
				//@}
				
				friend class ClassificationValidation;
				
				
				
		};
	}
}

#endif // CLASSIFICATION
