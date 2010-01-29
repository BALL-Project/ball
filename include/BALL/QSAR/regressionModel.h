/* regressionModel.h
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

#ifndef REGRESSION
#define REGRESSION

#ifndef MODELH
#include <BALL/QSAR/Model.h>
#endif

#ifndef REGVALIDATION
#include <BALL/QSAR/regressionValidation.h>
#endif

#include <fstream>

namespace BALL
{
	namespace QSAR
	{
		class BALL_EXPORT RegressionModel : public Model
		{
			public:
				/** @name Constructors and Destructors
				 */
				//@{
				/** constructur,
				@param q QSAR-wrapper object, from which the data for this model should be taken */
				RegressionModel(const QSARData& q);
				
				~RegressionModel();
				//@}
				
				
				/** @name Accessors
				 */
				//@{
				/** a ModelValidation object, that is used to validate this model and that will contain the results of the validations */
				RegressionValidation* validation;
				
				/** returns a const pointer to the matrix containing the coefficients obtained by Model.train() */
				const BALL::Matrix<double>* getTrainingResult() const; 
				
				virtual void saveToFile(string filename);
				
				virtual void readFromFile(string filename);
				
				void show();
				//@}
				
				
			protected:
				/** @name Attributes
				 */
				//@{
				/** BALL::Matrix<double> containing the coefficients obtained by Model.train().\n
				raining_result will have the following dimensions for the different types of models, with m=no of descriptors and c=no of modelled activities (=no of columns of Model.Y) : \n
				LinearModel : mxc \n
				KernelModel : nxc \n
				ALLModel : mxc \n
				FitModel : mxc \n
				SVMModel : m x (c*no of classes) \n
				SVRModel : m x (c*no of classes)    */
				BALL::Matrix<double> training_result_;
				
				// RowVector holding the regression constants (one value for each feature)
				Vector<double> offsets_;
				//@}
				
				
				/** @name Input and Output. The following methods can be used to implement the functions saveToFile() and readFromFile() in final classes derived from this base-class 
				 */
				//@{
				virtual void calculateOffsets() = 0;
				
				void readDescriptorInformationFromFile(ifstream& in, int no_descriptors, bool transformation, int no_coefficients);
				
				void saveDescriptorInformationToFile(ofstream& out);
				//@}
				
				
				friend class RegressionValidation;
				
		};
	}
}

#endif // REGRESSION
